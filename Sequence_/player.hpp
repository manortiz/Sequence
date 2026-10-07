#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "card.hpp"

class Player {
 public:
    // id is 1 or 2 and matches the occupant id stored in board cells
    Player(const std::string& name, int id, char chip);

    const std::string& name() const;
    int id() const;
    char chip() const;

    // Cards currently in hand
    const std::vector<Card>& hand() const;

    // Adds a card to hand
    void addCard(const Card& card);

    // Removes and returns the card at index (0-based)
    Card removeCard(std::size_t index);

    // Completed sequences so far
    int sequences() const;
    void addSequences(int count);

 private:
    std::string name_;
    int id_;
    char chip_;
    std::vector<Card> hand_;
    int sequences_;
};

#endif
