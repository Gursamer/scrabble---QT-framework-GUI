#include "Tile.h"

Tile::Tile(char letter, int score) : letter(letter), score(score) {}

char Tile::getLetter() const {
    return letter;
}

int Tile::getScore() const {
    return score;
}
