#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "card.hpp"
#include "cell.hpp"

struct Position {
    int row;
    int col;
};

bool operator==(const Position& a, const Position& b);

// Formats a position as a coordinate, e.g. {row 3, col 2} -> "C4"
std::string toString(const Position& pos);

// Parses a coordinate like "C4" or "j10" into out. Returns false if invalid.
bool parsePosition(const std::string& text, Position& out);

// The 10x10 Sequence board. Every non-jack card appears exactly twice and
// the four corners are free spaces that count for every player.
class Board {
 public:
    static constexpr int SIZE = 10;
    static constexpr int SEQUENCE_LENGTH = 5;

    // Sets up the standard board layout
    Board();

    // Cell getter
    const Cell& at(const Position& pos) const;

    // True if pos is on the board
    static bool inBounds(const Position& pos);

    // Chip symbol drawn for a player id
    static char chipSymbol(int player);

    // Open (unoccupied) cells showing this card. Always empty for jacks.
    std::vector<Position> openPositionsFor(const Card& card) const;

    // True if card is not a jack and both of its cells are taken
    bool isDeadCard(const Card& card) const;

    // True if a two-eyed jack may place a chip at pos
    bool canPlaceWild(const Position& pos) const;

    // True if a one-eyed jack played by player may remove the chip at pos
    bool canRemove(const Position& pos, int player) const;

    // All chips that player could remove with a one-eyed jack
    std::vector<Position> removablePositions(int player) const;

    // Places player's chip at pos
    void place(const Position& pos, int player);

    // Removes the chip at pos
    void remove(const Position& pos);

    // Finds new sequences that run through pos for player, locks their chips,
    // and returns how many were completed.
    int claimSequences(const Position& pos, int player);

    // Draws the board. Highlighted cells are wrapped in > <.
    void print(std::ostream& os,
               const std::vector<Position>& highlights = {}) const;

 private:
    // True if cell counts toward player's sequences (their chip or a corner)
    bool countsFor(const Position& pos, int player) const;

    Cell& cellAt(const Position& pos);

    Cell cells_[SIZE][SIZE];
};

#endif
