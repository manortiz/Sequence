#ifndef CARD_HPP_
#define CARD_HPP_

#include <string>

class Card {
   public: 
    // Disable default constructor
    Card();

    // Card Constructor 
    Card(std::string suit, std::string rank);

    // Gets suit of card 
    std::string suit() const; 

    // Gets rank of card
    std::string rank() const; 

 private: 
    std::string suit_;
    std::string rank_;
};

#endif
