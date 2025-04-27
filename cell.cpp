#include "cell.hpp"
#include "card.hpp"

Cell::Cell() : card_{"", ""} , occupied_{false}, occupant_{0} {

}

// Card getter
Card Cell::getCard() const {
    return card_;
}

// True if cell occupied
bool Cell::occupied() const {
    return occupied_;
}

// Occupant getter
int Cell::occupant() const {
    return occupant_;
}

void Cell::occupy() {
    occupied_ = true; 
}

void Cell::unoccupy() {
    occupied_ = false;
}