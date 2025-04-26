#include "card.hpp"
#include <string>

Card::Card() : suit_{""}, rank_{""} {
    // Placeholder card 
}

Card::Card(std::string suit, std::string rank) : suit_{suit}, rank_{rank} {
    // Nothing else to do here!
}

std::string Card::suit() const {
    return suit_;
}

std::string Card::rank() const {
    return rank_;
}

