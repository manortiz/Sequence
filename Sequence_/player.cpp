#include "player.hpp"

Player::Player(const std::string& name, int id, char chip)
    : name_{name}, id_{id}, chip_{chip}, sequences_{0} {
}

const std::string& Player::name() const {
    return name_;
}

int Player::id() const {
    return id_;
}

char Player::chip() const {
    return chip_;
}

const std::vector<Card>& Player::hand() const {
    return hand_;
}

void Player::addCard(const Card& card) {
    hand_.push_back(card);
}

Card Player::removeCard(std::size_t index) {
    Card card = hand_.at(index);
    hand_.erase(hand_.begin() + static_cast<std::ptrdiff_t>(index));
    return card;
}

int Player::sequences() const {
    return sequences_;
}

void Player::addSequences(int count) {
    sequences_ += count;
}
