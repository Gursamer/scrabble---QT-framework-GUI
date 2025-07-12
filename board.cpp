#include "board.h"
#include <stdexcept>

Board::Board() {
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            grid[i][j] = nullptr;
}

bool Board::isValidPlacement(int row, int col, const std::vector<Tile>& tiles, bool horizontal, bool isFirstTurn) {
    if (horizontal && col + tiles.size() > 15) return false;
    if (!horizontal && row + tiles.size() > 15) return false;

    bool connects = false;
    for (int i = 0; i < tiles.size(); ++i) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        if (grid[r][c] != nullptr && grid[r][c]->getLetter() != tiles[i].getLetter())
            return false;

        if (grid[r][c] != nullptr) connects = true;
    }

    if (isFirstTurn) {
        bool coversCenter = false;
        for (int i = 0; i < tiles.size(); ++i) {
            int r = row + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);
            if (r == 7 && c == 7) coversCenter = true;
        }
        return coversCenter;
    }

    return connects;
}

void Board::placeTiles(int row, int col, const std::vector<Tile>& tiles, bool horizontal) {
    for (int i = 0; i < tiles.size(); ++i) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (!grid[r][c])
            grid[r][c] = new Tile(tiles[i]);
    }
}

Tile* Board::getTileAt(int row, int col) {
    return grid[row][col];
}
