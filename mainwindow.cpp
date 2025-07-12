#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <cctype>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    promptPlayerSetup();
    updateBoard();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    boardLayout = new QGridLayout();
    mainLayout->addLayout(boardLayout);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputWord = new QLineEdit();
    inputRow = new QLineEdit();
    inputCol = new QLineEdit();
    inputDirection = new QLineEdit();

    inputWord->setPlaceholderText("Word");
    inputRow->setPlaceholderText("Row (1-15)");
    inputCol->setPlaceholderText("Col (1-15)");
    inputDirection->setPlaceholderText("Direction (H/V)");

    inputLayout->addWidget(inputWord);
    inputLayout->addWidget(inputRow);
    inputLayout->addWidget(inputCol);
    inputLayout->addWidget(inputDirection);
    mainLayout->addLayout(inputLayout);

    playButton = new QPushButton("Play Word");
    exchangeButton = new QPushButton("Exchange Tiles");
    passButton = new QPushButton("Pass Turn");
    resignButton = new QPushButton("Resign");

    mainLayout->addWidget(playButton);
    mainLayout->addWidget(exchangeButton);
    mainLayout->addWidget(passButton);
    mainLayout->addWidget(resignButton);

    playerTilesLabel = new QLabel();
    scoreLabel = new QLabel();
    turnLabel = new QLabel();
    mainLayout->addWidget(turnLabel);
    mainLayout->addWidget(scoreLabel);
    mainLayout->addWidget(playerTilesLabel);

    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(exchangeButton, &QPushButton::clicked, this, &MainWindow::onExchangeClicked);
    connect(passButton, &QPushButton::clicked, this, &MainWindow::onPassClicked);
    connect(resignButton, &QPushButton::clicked, this, &MainWindow::onResignClicked);
}

void MainWindow::promptPlayerSetup() {
    bool ok;
    int playerCount;
    do {
        playerCount = QInputDialog::getInt(this, "Player Count", "Enter number of players (2–4):", 2, 1, 10, 1, &ok);
        if (!ok) {
            QTimer::singleShot(0, this, SLOT(close()));
            return;
        }
        if (playerCount < 2 || playerCount > 4) {
            displayMessage("Please enter the number of players between 2 and 4.");
        }
    } while (playerCount < 2 || playerCount > 4);

    std::vector<std::string> names;
    for (int i = 0; i < playerCount; i++) {
        QString name = QInputDialog::getText(this, "Player Name", QString("Enter name for Player %1:").arg(i + 1), QLineEdit::Normal, "", &ok);
        if (!ok) {
            QTimer::singleShot(0, this, SLOT(close()));
            return;
        }
        names.push_back(name.toStdString());
    }

    try {
        game.loadDictionary("/Users/rithwik/scrabbleguiproject/dictionary.txt");
        game.setupPlayers(playerCount, names);
    } catch (const std::exception& e) {
        displayMessage(e.what());
    }
}

void MainWindow::updateBoard() {
    while (QLayoutItem* item = boardLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    for (int r = 0; r <= 15; r++) {
        for (int c = 0; c <= 15; c++) {
            QString label;
            if (r == 0 && c > 0) {
                label = QString::number(c);
            } else if (c == 0 && r > 0) {
                label = QString::number(r);
            } else if (r > 0 && c > 0) {
                Tile* tile = game.getBoard().getTileAt(r - 1, c - 1);
                if (tile) {
                    label = QChar(tile->getLetter());
                } else if (r == 8 && c == 8 && game.isFirstTurn()) {
                    label = "*";
                } else {
                    label = "";
                }
            }
            QPushButton* btn = new QPushButton(label);
            btn->setFixedSize(30, 30);
            btn->setEnabled(false);
            btn->setStyleSheet("background-color: white; color: black; font-weight: bold;");
            boardLayout->addWidget(btn, r, c);
        }
    }

    QString tileText = "Your Tiles: ";
    for (const auto& tile : game.getCurrentPlayer().getHand()) {
        tileText += QChar(tile.getLetter());
        tileText += " ";
    }

    turnLabel->setText("Current Player: " + QString::fromStdString(game.getCurrentPlayer().getName()));
    scoreLabel->setText("Score: " + QString::number(game.getCurrentPlayer().getScore()));
    playerTilesLabel->setText(tileText);
}

void MainWindow::onPlayClicked() {
    std::string word = inputWord->text().toStdString();
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    int row = inputRow->text().toInt() - 1;
    int col = inputCol->text().toInt() - 1;
    bool horizontal = inputDirection->text().toUpper() == "H";

    if (!game.validateWord(word)) {
        displayMessage("Invalid word! Not found in the dictionary");
        return;
    }

    std::string error;
    if (!game.playWord(row, col, word, horizontal, error)) {
        displayMessage(QString::fromStdString(error));
        return;
    }

    game.advanceTurn();
    inputWord->clear();
    inputRow->clear();
    inputCol->clear();
    inputDirection->clear();
    updateBoard();
}

void MainWindow::onExchangeClicked() {
    bool ok;
    QString toExchange = QInputDialog::getText(this, "Exchange Tiles", "Enter tiles to exchange:", QLineEdit::Normal, "", &ok);
    if (!ok || toExchange.isEmpty()) return;

    std::vector<Tile> tilesToExchange;
    for (char ch : toExchange.toStdString()) {
        if (isalpha(ch) || ch == '_')
            tilesToExchange.emplace_back(std::toupper(ch), 0);
    }

    game.exchangeTiles(tilesToExchange);
    game.advanceTurn();
    updateBoard();
}
void MainWindow::onPassClicked() {
    game.passTurn();
    updateBoard();
}

void MainWindow::onResignClicked() {
    std::string resignedName = game.getCurrentPlayer().getName();
    game.resignPlayer();

    const auto& players = game.getPlayers();

    QString message = QString::fromStdString(resignedName + " has resigned.\n");

    if (players.size() == 1) {
        //Check for tie:
        int topScore = players[0].getScore();
        std::vector<std::string> tiedPlayers = { players[0].getName() };
        message += "\nWinner: " + QString::fromStdString(tiedPlayers[0]) +
                   " with score " + QString::number(topScore);
        displayMessage(message);
        QTimer::singleShot(0, this, SLOT(close()));
        return;
    }
    int topScore = 0;
    std::vector<std::string> topPlayers;
    for (const auto& p : players) {
        if (p.getScore() > topScore) {
            topScore = p.getScore();
            topPlayers = { p.getName() };
        } else if (p.getScore() == topScore) {
            topPlayers.push_back(p.getName());
        }
    }
    if (players.size() == 1) {
        message += "\nWinner: " + QString::fromStdString(players[0].getName()) +
                   " with score " + QString::number(players[0].getScore());
    } else if (topPlayers.size() > 1) {
        message += "\nIt's a tie between: ";
        for (const auto& name : topPlayers) {
            message += QString::fromStdString(name) + " ";
        }
        message += "\nScore: " + QString::number(topScore);
    } else {
        message += "\nWinner: " + QString::fromStdString(topPlayers[0]) +
                   " with score " + QString::number(topScore);
    }

    message += "\n\nRemaining Players:\n";
    for (const auto& p : players) {
        message += QString::fromStdString(p.getName()) + ": " +
                   QString::number(p.getScore()) + "\n";
    }

    displayMessage(message);
    updateBoard();
}
void MainWindow::displayMessage(const QString& msg) {
    QMessageBox::information(this, "Info", msg);
}
