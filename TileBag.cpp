#include "TileBag.h"
#include <algorithm>
#include <vector>
#include <random>

    TileBag::TileBag() {
        // Format: {Letter, {Score, Quantity}}
        std::vector<std::pair<char, std::pair<int, int>>> distribution = {
            {'A', {1, 9}}, {'B', {3, 2}}, {'C', {3, 2}}, {'D', {2, 4}}, {'E', {1, 12}}, {'F', {4, 2}}, {'G', {2, 3}},
            {'H', {4, 2}}, {'I', {1, 9}}, {'J', {8, 1}}, {'K', {5, 1}}, {'L', {1, 4}}, {'M', {3, 2}}, {'N', {1, 6}},
            {'O', {1, 8}}, {'P', {3, 2}}, {'Q', {10, 1}}, {'R', {1, 6}}, {'S', {1, 4}}, {'T', {1, 6}}, {'U', {1, 4}},
            {'V', {4, 2}}, {'W', {4, 2}}, {'X', {8, 1}}, {'Y', {4, 2}}, {'Z', {10, 1}}, {'_', {0, 2}}
        };

        for (const auto& entry : distribution) {
            char letter = entry.first;
            int score = entry.second.first;
            int count = entry.second.second;
            for (int i = 0; i < count; i++) {
                tiles.emplace_back(letter, score);
            }
        }

        std::shuffle(tiles.begin(), tiles.end(), std::mt19937{std::random_device{}()});
    }

std::vector<Tile> TileBag::drawTiles(int count) {
    std::vector<Tile> drawn;
    for (int i = 0; i < count && !tiles.empty(); i++) {
        drawn.push_back(tiles.back());
        tiles.pop_back();
    }
    return drawn;
}
void TileBag::returnTile(const Tile& tile){
    tiles.push_back(tile);
    std::shuffle(tiles.begin(), tiles.end(), std::mt19937{std::random_device{}()});
}

int TileBag::tilesRemaining() const {
    return tiles.size();
}
