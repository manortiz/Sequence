#ifndef GAME_HPP_
#define GAME_HPP_

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "board.hpp"
#include "deck.hpp"
#include "player.hpp"

// Two-player, hot-seat text version of Sequence.
class Game {
 public:
    static constexpr int HAND_SIZE = 7;          // 2-player hand size
    static constexpr int SEQUENCES_TO_WIN = 2;   // 2-player goal

    Game(std::istream& in = std::cin, std::ostream& out = std::cout);

    // Runs the whole game until someone wins or a player quits
    void run();

 private:
    enum class MoveResult { Played, Cancelled };

    void setupPlayers();
    void dealHands();

    // Plays one turn. Returns false if the game is over.
    bool takeTurn(Player& current, Player& opponent);

    MoveResult playNormalCard(Player& current, std::size_t index);
    MoveResult playTwoEyedJack(Player& current, std::size_t index);
    MoveResult playOneEyedJack(Player& current, std::size_t index);

    // Puts current's chip at pos using the card at index, then scores
    // any new sequences and draws a replacement card
    void placeChip(Player& current, std::size_t index, const Position& pos);

    // Discards the card at index and draws a replacement
    void replaceCard(Player& current, std::size_t index);

    // Swaps a dead card for a new one. Sets status to a message for the player
    // and returns true if the swap happened.
    bool discardDeadCard(Player& current, std::size_t index, std::string& status);

    // True if current has at least one card they can legally play
    bool hasPlayableCard(const Player& current) const;

    void showState(const Player& current,
                   const std::vector<Position>& highlights = {}) const;
    void showHand(const Player& current) const;
    void showHelp() const;

    // Reads one trimmed line. Throws if input is closed.
    std::string prompt(const std::string& message);
    void waitForEnter(const std::string& message);
    void clearScreen() const;

    std::istream& in_;
    std::ostream& out_;
    Board board_;
    Deck deck_;
    std::vector<Player> players_;

    // Description and location of the most recent move, shown to both players
    std::string lastMove_;
    Position lastPos_;
    bool hasLastMove_;
};

#endif
