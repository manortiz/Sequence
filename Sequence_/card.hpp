#ifndef CARD_HPP_
#define CARD_HPP_

#include <ostream>
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

    // True if card is any jack
    bool isJack() const;

    // Two-eyed jacks (Diamonds, Clubs) are wild: place a chip anywhere open
    bool isTwoEyedJack() const;

    // One-eyed jacks (Hearts, Spades) remove an opponent's chip
    bool isOneEyedJack() const;

    // Short name of card, e.g. "QS", "10H"
    std::string toString() const;

    bool operator==(const Card& other) const;
    bool operator!=(const Card& other) const;

    Suit suit_;
    Rank rank_;
};

// Parses a short card name like "QS" or "10h" into out. Returns false if invalid.
bool parseCard(const std::string& text, Card& out);

std::ostream& operator<<(std::ostream& os, const Card& card);

#endif
