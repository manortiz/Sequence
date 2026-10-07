#include "card.hpp"
#include <string>


Card::Card(Suit suit, Rank rank) : suit_{suit}, rank_{rank} {
    // Done! 
}

Suit Card::suit() const {
    return suit_;
}

Rank Card::rank() const {
    return rank_;
}

