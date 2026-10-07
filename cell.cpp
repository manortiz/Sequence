#include "cell.hpp"
#include "card.hpp"

Cell::Cell() : card_{Suit::Hearts, Rank::Two}, occupant_{0} {
    
}

// Card getter
Card Cell::getCard() const {
    return card_;
}

// True if cell occupied
bool Cell::occupant() const {
    return occupant_;
}

void Cell::setCard(const Card& newCard) {
    card_ = newCard;
}

void Cell::occupy() {
    occupant_ = 1; 
}

void Cell::unoccupy() {
    occupant_ = 0;
}

void Cell::setOccupant(int newOccupant) {
    occupant_ = newOccupant;
}

void Cell::removeOccupant() {
    occupant_ = 0;
}