#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"
#include <vector>

class Board {
public:
    Board();
    bool isValidPlacement(int row, int col, const std::vector<Tile>& tiles, bool horizontal, bool isFirstTurn);
    void placeTiles(int row, int col, const std::vector<Tile>& tiles, bool horizontal);
    Tile* getTileAt(int row, int col);

private:
    Tile* grid[15][15];
};

#endif
