#ifndef DECK_HPP_
#define DECK_HPP_

#include <cstddef>
#include <random>
#include <vector>

#include "card.hpp"

// Draw pile made of two standard 52-card decks (104 cards), plus a discard pile.
class Deck {
 public:
    // Builds and shuffles two full decks
    Deck();

    // Draws the top card into out. If the draw pile is empty, the discard
    // pile is shuffled back in. Returns false if no cards are left at all.
    bool draw(Card& out);

    // Puts a card on the discard pile
    void discard(const Card& card);

    // Number of cards left in the draw pile
    std::size_t size() const;

 private:
    std::vector<Card> cards_;
    std::vector<Card> discards_;
    std::mt19937 rng_;
};

#endif
