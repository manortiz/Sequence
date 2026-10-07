#include "card.hpp"

#include <cctype>
#include <string>

namespace {

const char* const RANK_NAMES[] = {"2", "3", "4",  "5", "6", "7", "8",
                                  "9", "10", "J", "Q", "K", "A"};

// Unicode characters for the suits: Hearts, Diamonds, Clubs, Spades, respectively.
// The U prefix makes each literal a single code point; without it '\u2665' is a
// multi-character constant holding the packed UTF-8 bytes.
const int SUIT_NAMES[] = {U'\u2665', U'\u2666', U'\u2663', U'\u2660'};

// Plain-letter suits, used when parsing card names like "QS" or "10H"
const char SUIT_LETTERS[] = {'H', 'D', 'C', 'S'};
}  // namespace

// Encodes a Unicode code point as UTF-8
std::string toUtf8(int codePoint) {
    std::string out;
    if (codePoint < 0x80) {
        out += static_cast<char>(codePoint);
    } else if (codePoint < 0x800) {
        out += static_cast<char>(0xC0 | (codePoint >> 6));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint < 0x10000) {
        out += static_cast<char>(0xE0 | (codePoint >> 12));
        out += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (codePoint >> 18));
        out += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    }
    return out;
}

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
           toUtf8(SUIT_NAMES[static_cast<int>(suit_)]);
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
        if (SUIT_LETTERS[s] != suitChar) {
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

std::size_t displayWidth(const std::string& text) {
    std::size_t width = 0;
    for (char c : text) {
        // Count every byte except UTF-8 continuation bytes (10xxxxxx)
        if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) {
            ++width;
        }
    }
    return width;
}
