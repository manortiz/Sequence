#include "card.hpp"

#include <cctype>
#include <string>

namespace {

const char* const RANK_NAMES[] = {"2", "3", "4",  "5", "6", "7", "8",
                                  "9", "10", "J", "Q", "K", "A"};
const char SUIT_NAMES[] = {'H', 'D', 'C', 'S'};

}  // namespace

Card::Card(Suit suit, Rank rank) : suit_{suit}, rank_{rank} {
    // Done!
}

Suit Card::suit() const {
    return suit_;
}

Rank Card::rank() const {
    return rank_;
}

bool Card::isJack() const {
    return rank_ == Rank::Jack;
}

bool Card::isTwoEyedJack() const {
    return isJack() && (suit_ == Suit::Diamonds || suit_ == Suit::Clubs);
}

bool Card::isOneEyedJack() const {
    return isJack() && (suit_ == Suit::Hearts || suit_ == Suit::Spades);
}

std::string Card::toString() const {
    return RANK_NAMES[static_cast<int>(rank_)] +
           std::string(1, SUIT_NAMES[static_cast<int>(suit_)]);
}

bool Card::operator==(const Card& other) const {
    return suit_ == other.suit_ && rank_ == other.rank_;
}

bool Card::operator!=(const Card& other) const {
    return !(*this == other);
}

bool parseCard(const std::string& text, Card& out) {
    if (text.size() < 2) {
        return false;
    }

    std::string upper;
    for (char c : text) {
        upper += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    std::string rankText = upper.substr(0, upper.size() - 1);
    char suitChar = upper.back();
    if (rankText == "T") {
        rankText = "10";
    }

    for (int s = 0; s < 4; ++s) {
        if (SUIT_NAMES[s] != suitChar) {
            continue;
        }
        for (int r = 0; r < 13; ++r) {
            if (rankText == RANK_NAMES[r]) {
                out = Card(static_cast<Suit>(s), static_cast<Rank>(r));
                return true;
            }
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    return os << card.toString();
}
