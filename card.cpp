#include "card.hpp"
#include <string>

Card::Card() : suit_{Suit::Hearts}, rank_{Rank::Two} {
    // Placeholder card 
}

Card::Card(Suit suit, Rank rank) : suit_{suit}, rank_{rank} {
    // Done! 
}

Card::Suit Card::suit() const {
    return suit_;
}

Card::Rank Card::rank() const {
    return rank_;
}

