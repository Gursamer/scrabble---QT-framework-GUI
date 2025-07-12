#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Tile.h"
#include "TileBag.h"

class Player {
public:
    Player(const std::string& name);

    void drawTiles(TileBag& bag, int count);
    void addScore(int points);
    const std::string& getName() const;
    int getScore() const;
    std::vector<Tile>& getHand();
    void setHand(const std::vector<Tile>& newHand);

private:
    std::string name;
    int score;
    std::vector<Tile> hand;
};

#endif
