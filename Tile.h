#ifndef TILE_H
#define TILE_H

class Tile {
public:
    Tile(char letter = ' ', int score = 0);

    char getLetter() const;
    int getScore() const;

private:
    char letter;
    int score;
};

#endif
