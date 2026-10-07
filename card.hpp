#ifndef CARD_HPP_
#define CARD_HPP_

#include <string>

enum class Suit {
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

enum class Rank {
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

struct Card {
    // Card Constructor 
    Card(Suit suit, Rank rank);

    // Gets suit of card 
    Suit suit() const; 

    // Gets rank of card
    Rank rank() const; 

    Suit suit_;
    Rank rank_;
};

#endif
