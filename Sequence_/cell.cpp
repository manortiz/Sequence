#include "cell.hpp"
#include "card.hpp"

Cell::Cell()
    : card_{Suit::Hearts, Rank::Two},
      corner_{false},
      occupant_{0},
      inSequence_{false} {
}

// Card getter
Card Cell::getCard() const {
    return card_;
}

void Cell::setCard(const Card& newCard) {
    card_ = newCard;
}

bool Cell::isCorner() const {
    return corner_;
}

void Cell::makeCorner() {
    corner_ = true;
}

int Cell::occupant() const {
    return occupant_;
}

bool Cell::isOccupied() const {
    return occupant_ != 0;
}

void Cell::setOccupant(int newOccupant) {
    occupant_ = newOccupant;
}

void Cell::removeOccupant() {
    occupant_ = 0;
    inSequence_ = false;
}

bool Cell::inSequence() const {
    return inSequence_;
}

void Cell::lockInSequence() {
    inSequence_ = true;
}
