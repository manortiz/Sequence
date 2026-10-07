#ifndef CELL_HPP_
#define CELL_HPP_

#include "card.hpp"

class Cell {
 public:
    // Default constructor, empty cell
    Cell();

    // Card getter
    Card getCard() const;

    // Card setter
    void setCard(const Card& newCard);

    // True if cell is one of the four free corners
    bool isCorner() const;

    // Mark cell as a free corner
    void makeCorner();

    // Id of the player whose chip is here (0 if empty)
    int occupant() const;

    // True if a chip is on this cell
    bool isOccupied() const;

    // Occupant setter
    void setOccupant(int newOccupant);

    // Occupant remover
    void removeOccupant();

    // True if the chip here is part of a completed sequence
    bool inSequence() const;

    // Mark chip as part of a completed sequence (can no longer be removed)
    void lockInSequence();

 private:
    Card card_;
    bool corner_;
    int occupant_;
    bool inSequence_;
};

#endif
