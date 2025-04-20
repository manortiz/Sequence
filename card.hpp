#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED

#include <string>

class Card {

 private: 
    std::string suit_;
    std::string rank_;

 public: 
    Card() = delete;

    /**
     *  Card constructor 
    */ 
    Card(std::string suit, std::string rank);

};

#endif