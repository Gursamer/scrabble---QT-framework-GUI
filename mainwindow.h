#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayClicked();
    void onExchangeClicked();
    void onPassClicked();
    void onResignClicked();

private:
    void setupUI();
    void updateBoard();
    void displayMessage(const QString& msg);
    void promptPlayerSetup();

    QWidget *centralWidget;
    QGridLayout *boardLayout;
    QLineEdit *inputWord;
    QLineEdit *inputRow;
    QLineEdit *inputCol;
    QLineEdit *inputDirection;
    QLabel *playerTilesLabel;
    QLabel *scoreLabel;
    QLabel *turnLabel;

    QPushButton *playButton;
    QPushButton *exchangeButton;
    QPushButton *passButton;
    QPushButton *resignButton;

    Game game;
};

#endif
