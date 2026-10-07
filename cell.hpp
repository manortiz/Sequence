#ifndef CELL_HPP_
#define CELL_HPP_

#include "card.hpp"

class Cell {
 public: 
    // Default constructor, empty cell
    Cell();

    // Card getter
    Card getCard() const;

    // True if cell occupied
    bool occupant() const;

    // Card setter
    void setCard(const Card& newCard);

    // Mark cell as occupied
    void occupy();

    // Mark cell as unoccupied
    void unoccupy();

    // Occupant setter
    void setOccupant(int newOccupant); 

    // Occupant remover
    void removeOccupant();

 private:
    Card card_;
    int occupant_;
};

#endif
