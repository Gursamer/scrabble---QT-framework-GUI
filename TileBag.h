#ifndef TILEBAG_H
#define TILEBAG_H

#include <vector>
#include <random>
#include "Tile.h"

class TileBag {
public:
    TileBag();
    std::vector<Tile> drawTiles(int count);
    void returnTile(const Tile& tile);
    int tilesRemaining() const;

private:
    std::vector<Tile> tiles;
};

#endif
