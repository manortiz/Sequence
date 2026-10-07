#include "deck.hpp"

#include <algorithm>

Deck::Deck() : rng_{std::random_device{}()} {
    for (int copy = 0; copy < 2; ++copy) {
        for (int s = 0; s < 4; ++s) {
            for (int r = 0; r < 13; ++r) {
                cards_.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
            }
        }
    }
    std::shuffle(cards_.begin(), cards_.end(), rng_);
}

bool Deck::draw(Card& out) {
    if (cards_.empty()) {
        if (discards_.empty()) {
            return false;
        }
        cards_.swap(discards_);
        std::shuffle(cards_.begin(), cards_.end(), rng_);
    }
    out = cards_.back();
    cards_.pop_back();
    return true;
}

void Deck::discard(const Card& card) {
    discards_.push_back(card);
}

std::size_t Deck::size() const {
    return cards_.size();
}
