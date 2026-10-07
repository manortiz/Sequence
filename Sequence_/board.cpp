#include "board.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <stdexcept>

namespace {

// Standard Sequence board layout. "**" marks the free corners.
const char* const LAYOUT[Board::SIZE][Board::SIZE] = {
    {"**",  "2S",  "3S",  "4S",  "5S",  "6S",  "7S",  "8S",  "9S",  "**"},
    {"6C",  "5C",  "4C",  "3C",  "2C",  "AH",  "KH",  "QH",  "10H", "10S"},
    {"7C",  "AS",  "2D",  "3D",  "4D",  "5D",  "6D",  "7D",  "9H",  "QS"},
    {"8C",  "KS",  "6C",  "5C",  "4C",  "3C",  "2C",  "8D",  "8H",  "KS"},
    {"9C",  "QS",  "7C",  "6H",  "5H",  "4H",  "AH",  "9D",  "7H",  "AS"},
    {"10C", "10S", "8C",  "7H",  "2H",  "3H",  "KH",  "10D", "6H",  "2D"},
    {"QC",  "9S",  "9C",  "8H",  "9H",  "10H", "QH",  "QD",  "5H",  "3D"},
    {"KC",  "8S",  "10C", "QC",  "KC",  "AC",  "AD",  "KD",  "4H",  "4D"},
    {"AC",  "7S",  "6S",  "5S",  "4S",  "3S",  "2S",  "2H",  "3H",  "5D"},
    {"**",  "AD",  "KD",  "QD",  "10D", "9D",  "8D",  "7D",  "6D",  "**"},
};

// Row/column steps for horizontal, vertical, and both diagonals
const int DIRECTIONS[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

}  // namespace

bool operator==(const Position& a, const Position& b) {
    return a.row == b.row && a.col == b.col;
}

std::string toString(const Position& pos) {
    return std::string(1, static_cast<char>('A' + pos.col)) +
           std::to_string(pos.row + 1);
}

bool parsePosition(const std::string& text, Position& out) {
    if (text.size() < 2 || text.size() > 3) {
        return false;
    }
    char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(text[0])));
    if (letter < 'A' || letter >= 'A' + Board::SIZE) {
        return false;
    }
    std::string digits = text.substr(1);
    if (!std::all_of(digits.begin(), digits.end(),
                     [](char c) { return std::isdigit(static_cast<unsigned char>(c)); })) {
        return false;
    }
    int row = std::stoi(digits);
    if (row < 1 || row > Board::SIZE) {
        return false;
    }
    out = Position{row - 1, letter - 'A'};
    return true;
}

Board::Board() {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            std::string name = LAYOUT[r][c];
            if (name == "**") {
                cells_[r][c].makeCorner();
                continue;
            }
            Card card{Suit::Hearts, Rank::Two};
            if (!parseCard(name, card)) {
                throw std::logic_error("Bad card in board layout: " + name);
            }
            cells_[r][c].setCard(card);
        }
    }
}

const Cell& Board::at(const Position& pos) const {
    return cells_[pos.row][pos.col];
}

Cell& Board::cellAt(const Position& pos) {
    return cells_[pos.row][pos.col];
}

bool Board::inBounds(const Position& pos) {
    return pos.row >= 0 && pos.row < SIZE && pos.col >= 0 && pos.col < SIZE;
}

char Board::chipSymbol(int player) {
    return player == 1 ? 'X' : 'O';
}

std::vector<Position> Board::openPositionsFor(const Card& card) const {
    std::vector<Position> open;
    if (card.isJack()) {
        return open;
    }
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            const Cell& cell = cells_[r][c];
            if (!cell.isCorner() && !cell.isOccupied() && cell.getCard() == card) {
                open.push_back(Position{r, c});
            }
        }
    }
    return open;
}

bool Board::isDeadCard(const Card& card) const {
    return !card.isJack() && openPositionsFor(card).empty();
}

bool Board::canPlaceWild(const Position& pos) const {
    return inBounds(pos) && !at(pos).isCorner() && !at(pos).isOccupied();
}

bool Board::canRemove(const Position& pos, int player) const {
    if (!inBounds(pos)) {
        return false;
    }
    const Cell& cell = at(pos);
    return cell.isOccupied() && cell.occupant() != player && !cell.inSequence();
}

std::vector<Position> Board::removablePositions(int player) const {
    std::vector<Position> removable;
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (canRemove(Position{r, c}, player)) {
                removable.push_back(Position{r, c});
            }
        }
    }
    return removable;
}

void Board::place(const Position& pos, int player) {
    cellAt(pos).setOccupant(player);
}

void Board::remove(const Position& pos) {
    cellAt(pos).removeOccupant();
}

bool Board::countsFor(const Position& pos, int player) const {
    if (!inBounds(pos)) {
        return false;
    }
    const Cell& cell = at(pos);
    return cell.isCorner() || cell.occupant() == player;
}

int Board::claimSequences(const Position& pos, int player) {
    int found = 0;
    for (const auto& dir : DIRECTIONS) {
        // Try every 5-long window along this line that includes pos
        for (int offset = -(SEQUENCE_LENGTH - 1); offset <= 0; ++offset) {
            bool complete = true;
            int alreadyLocked = 0;
            for (int i = 0; i < SEQUENCE_LENGTH; ++i) {
                Position p{pos.row + (offset + i) * dir[0],
                           pos.col + (offset + i) * dir[1]};
                if (!countsFor(p, player)) {
                    complete = false;
                    break;
                }
                if (at(p).inSequence()) {
                    ++alreadyLocked;
                }
            }
            // A new sequence may share at most one chip with an existing one
            if (!complete || alreadyLocked > 1) {
                continue;
            }
            for (int i = 0; i < SEQUENCE_LENGTH; ++i) {
                Position p{pos.row + (offset + i) * dir[0],
                           pos.col + (offset + i) * dir[1]};
                if (!at(p).isCorner()) {
                    cellAt(p).lockInSequence();
                }
            }
            ++found;
        }
    }
    return found;
}

void Board::print(std::ostream& os, const std::vector<Position>& highlights) const {
    os << "     ";
    for (int c = 0; c < SIZE; ++c) {
        os << "  " << static_cast<char>('A' + c) << "  ";
    }
    os << "\n    +" << std::string(SIZE * 5, '-') << "+\n";

    for (int r = 0; r < SIZE; ++r) {
        os << std::setw(3) << (r + 1) << " |";
        for (int c = 0; c < SIZE; ++c) {
            Position pos{r, c};
            const Cell& cell = at(pos);
            bool lit = std::find(highlights.begin(), highlights.end(), pos) !=
                       highlights.end();

            std::string content;
            if (cell.isCorner()) {
                content = "**";
            } else if (cell.isOccupied()) {
                char chip = chipSymbol(cell.occupant());
                content = cell.inSequence() ? std::string{'[', chip, ']'}
                                            : std::string{'(', chip, ')'};
            } else {
                content = cell.getCard().toString();
            }

            // Pad by display width, not bytes: suit symbols are multi-byte UTF-8
            std::string shown = (lit ? ">" : " ") + content + (lit ? "<" : " ");
            os << shown << std::string(5 - displayWidth(shown), ' ');
        }
        os << "| " << (r + 1) << "\n";
    }

    os << "    +" << std::string(SIZE * 5, '-') << "+\n     ";
    for (int c = 0; c < SIZE; ++c) {
        os << "  " << static_cast<char>('A' + c) << "  ";
    }
    os << "\n";
}
