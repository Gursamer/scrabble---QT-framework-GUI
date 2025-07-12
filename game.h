#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "board.h"
#include "TileBag.h"
#include <vector>
#include <unordered_set>
#include <string>

class Game {
public:
    Game();

    void loadDictionary(const std::string& filename);
    void setupPlayers(int count, const std::vector<std::string>& names);

    bool validateWord(const std::string& word) const;
    bool playWord(int row, int col, const std::string& word, bool horizontal, std::string& error);

    void exchangeTiles(std::vector<Tile>& tilesToExchange);
    void passTurn();
    void resignPlayer();
    void advanceTurn();

    Board& getBoard();
    Player& getCurrentPlayer();
    const std::vector<Player>& getPlayers() const;
    TileBag& getTileBag();

    bool isFirstTurn() const;

private:
    std::vector<Player> players;
    int currentPlayerIndex;
    bool firstTurn;
    Board board;
    TileBag bag;
    std::unordered_set<std::string> dictionary;
    int scoreLessTurnCount;
};

#endif
