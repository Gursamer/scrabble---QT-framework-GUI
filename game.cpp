#include "game.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>

Game::Game() : currentPlayerIndex(0), firstTurn(true) {}

void Game::loadDictionary(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open dictionary file");
    std::string word;
    while (file >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary.insert(word);
    }
}

void Game::setupPlayers(int count, const std::vector<std::string>& names) {
    if (count < 2 || count > 4) throw std::invalid_argument("Players must be between 2 and 4");
    players.clear();
    for (int i = 0; i < count; ++i) {
        players.emplace_back(names[i]);
        players.back().drawTiles(bag, 7);
    }
    currentPlayerIndex = 0;
}

bool Game::validateWord(const std::string& word) const {
    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return dictionary.count(lower) > 0;
}

bool Game::playWord(int row, int col, const std::string& word, bool horizontal, std::string& error) {
    if (isFirstTurn()) {
        if (!(row == 7 && col <= 7 && ((horizontal && col + word.size() > 7) || (!horizontal && row + word.size() > 7)))) {
            error = "First word must cross the center tile (8,8).";
            return false;
        }
    }

    std::vector<Tile> tiles;
    for (char ch : word) {
        tiles.emplace_back(ch, 1);  // scoring handled later
    }

    if (!board.isValidPlacement(row, col, tiles, horizontal, isFirstTurn())) {
        if (isFirstTurn())
            error = "First word must pass through the center tile (8,8).";
        else
            error = "Word must connect to existing tiles.";
        return false;
    }
    auto& hand = getCurrentPlayer().getHand();
    for (char ch : word) {
        auto it = std::find_if(hand.begin(), hand.end(), [&](const Tile& t) {
            return t.getLetter() == ch;
        });
        if (it != hand.end()) {
            hand.erase(it);
        }
    }

    board.placeTiles(row, col, tiles, horizontal);
    getCurrentPlayer().addScore(word.length());
    getCurrentPlayer().drawTiles(bag, 7 - getCurrentPlayer().getHand().size());

    firstTurn = false;
    return true;
}
//Exchanging tiles
void Game::exchangeTiles(std::vector<Tile>& tilesToExchange) {
    auto& hand = getCurrentPlayer().getHand();
    std::vector<Tile> actuallyExchanged;
        for (const auto& t : tilesToExchange) {
        auto it = std::find_if(hand.begin(), hand.end(), [&](const Tile& x) {
            return x.getLetter() == t.getLetter();
        });
        if (it != hand.end()) {
            bag.returnTile(*it);
            actuallyExchanged.push_back(t);
            hand.erase(it);
        }
    }
    auto newTiles = bag.drawTiles(actuallyExchanged.size());
    hand.insert(hand.end(), newTiles.begin(), newTiles.end());

    scoreLessTurnCount++;
}

void Game::passTurn() {
    advanceTurn();
}

void Game::resignPlayer() {
    players.erase(players.begin() + currentPlayerIndex);
    if (players.empty()) return;
    currentPlayerIndex %= players.size();
}

void Game::advanceTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

Board& Game::getBoard() {
    return board;
}
Player& Game::getCurrentPlayer() {
    return players[currentPlayerIndex];
}
const std::vector<Player>& Game::getPlayers() const {
    return players;
}
TileBag& Game::getTileBag() {
    return bag;
}
bool Game::isFirstTurn() const {
    return firstTurn;
}
