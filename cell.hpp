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
    bool occupied() const;

    // Occupant getter
    int occupant() const; 

    void setCard(Card& newCard);

    void occupy();

    void unoccupy();

    
 private:
    Card card_;
    bool occupied_;
    int occupant_;
};

#endif
