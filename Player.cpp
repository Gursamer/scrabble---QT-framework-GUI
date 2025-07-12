#include "Player.h"

Player::Player(const std::string& name) : name(name), score(0) {}

void Player::drawTiles(TileBag& bag, int count) {
    std::vector<Tile> newTiles = bag.drawTiles(count);
    hand.insert(hand.end(), newTiles.begin(), newTiles.end());
}

void Player::addScore(int points) {
    score += points;
}

const std::string& Player::getName() const {
    return name;
}

int Player::getScore() const {
    return score;
}

std::vector<Tile>& Player::getHand() {
    return hand;
}

void Player::setHand(const std::vector<Tile>& newHand) {
    hand = newHand;
}
