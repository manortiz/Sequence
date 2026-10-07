#include "game.hpp"

#include <algorithm>
#include <cctype>

namespace {

// Thrown when stdin closes so the game can exit cleanly
struct InputClosed {};

std::string trim(const std::string& text) {
    std::size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    std::size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

std::string toLower(std::string text) {
    for (char& c : text) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return text;
}

// Parses a 1-based menu number into a 0-based index below count
bool parseIndex(const std::string& text, std::size_t count, std::size_t& out) {
    if (text.empty() || text.size() > 3 ||
        !std::all_of(text.begin(), text.end(),
                     [](char c) { return std::isdigit(static_cast<unsigned char>(c)); })) {
        return false;
    }
    int value = std::stoi(text);
    if (value < 1 || static_cast<std::size_t>(value) > count) {
        return false;
    }
    out = static_cast<std::size_t>(value - 1);
    return true;
}

bool contains(const std::vector<Position>& positions, const Position& pos) {
    return std::find(positions.begin(), positions.end(), pos) != positions.end();
}

bool isBack(const std::string& line) {
    std::string lower = toLower(line);
    return lower == "b" || lower == "back";
}

}  // namespace

Game::Game(std::istream& in, std::ostream& out)
    : in_{in}, out_{out}, lastPos_{0, 0}, hasLastMove_{false} {
}

void Game::run() {
    try {
        clearScreen();
        out_ << "==============================\n"
             << "     S E Q U E N C E\n"
             << "==============================\n"
             << "Two players take turns at the same keyboard.\n"
             << "Get " << SEQUENCES_TO_WIN << " sequences of five chips in a row to win.\n\n";
        setupPlayers();
        dealHands();

        std::size_t turn = 0;
        while (takeTurn(players_[turn % 2], players_[(turn + 1) % 2])) {
            ++turn;
        }
    } catch (const InputClosed&) {
        out_ << "\nInput closed. Goodbye!\n";
    }
}

void Game::setupPlayers() {
    for (int id = 1; id <= 2; ++id) {
        std::string name =
            prompt("Name for Player " + std::to_string(id) + " (chip " +
                   Board::chipSymbol(id) + "): ");
        if (name.empty()) {
            name = "Player " + std::to_string(id);
        }
        players_.emplace_back(name, id, Board::chipSymbol(id));
    }
}

void Game::dealHands() {
    for (int i = 0; i < HAND_SIZE; ++i) {
        for (Player& player : players_) {
            Card card{Suit::Hearts, Rank::Two};
            if (deck_.draw(card)) {
                player.addCard(card);
            }
        }
    }
}

bool Game::takeTurn(Player& current, Player& opponent) {
    clearScreen();
    waitForEnter("Pass the keyboard to " + current.name() + " (" + current.chip() +
                 ").\n" + current.name() + ", press Enter when " +
                 opponent.name() + " isn't looking...");

    bool swappedDeadCard = false;
    std::string status;
    while (true) {
        clearScreen();
        showState(current);
        if (!status.empty()) {
            out_ << "\n" << status << "\n";
            status.clear();
        }

        bool stuck = !hasPlayableCard(current);
        out_ << "\nEnter a card number to play it, 'd <number>' to swap a dead card,\n"
             << "'help' for the rules, or 'quit' to end the game.\n";
        if (stuck) {
            out_ << "You have no playable cards. You may 'pass'.\n";
        }

        std::string line = toLower(prompt("> "));
        if (line.empty()) {
            continue;
        }

        if (line == "q" || line == "quit") {
            std::string confirm = toLower(prompt("Really quit the game? (y/n) "));
            if (confirm == "y" || confirm == "yes") {
                out_ << "\n" << current.name() << " quit. Thanks for playing!\n";
                return false;
            }
            continue;
        }

        if (line == "h" || line == "help" || line == "?") {
            clearScreen();
            showHelp();
            waitForEnter("\nPress Enter to return to the game...");
            continue;
        }

        if (line == "pass") {
            if (!stuck) {
                status = "You have a playable card, so you can't pass.";
                continue;
            }
            lastMove_ = current.name() + " had no playable cards and passed.";
            hasLastMove_ = false;
            break;
        }

        if (line[0] == 'd') {
            std::size_t index = 0;
            if (!parseIndex(trim(line.substr(1)), current.hand().size(), index)) {
                status = "To swap a dead card, type 'd' and its number, e.g. 'd 3'.";
                continue;
            }
            if (swappedDeadCard) {
                status = "You can only swap one dead card per turn.";
                continue;
            }
            swappedDeadCard = discardDeadCard(current, index, status);
            continue;
        }

        std::size_t index = 0;
        if (!parseIndex(line, current.hand().size(), index)) {
            status = "Please enter a card number from 1 to " +
                     std::to_string(current.hand().size()) + ".";
            continue;
        }

        Card card = current.hand()[index];
        MoveResult result = MoveResult::Cancelled;
        if (card.isTwoEyedJack()) {
            result = playTwoEyedJack(current, index);
        } else if (card.isOneEyedJack()) {
            result = playOneEyedJack(current, index);
        } else if (board_.isDeadCard(card)) {
            status = card.toString() + " is a dead card (both of its spaces are taken). " +
                     "Swap it with 'd " + std::to_string(index + 1) + "'.";
        } else {
            result = playNormalCard(current, index);
        }

        if (result == MoveResult::Played) {
            break;
        }
    }

    clearScreen();
    showState(current);

    if (current.sequences() >= SEQUENCES_TO_WIN) {
        out_ << "\n*****************************************\n"
             << "  " << current.name() << " (" << current.chip() << ") WINS with "
             << current.sequences() << " sequences!\n"
             << "*****************************************\n";
        return false;
    }

    waitForEnter("\nPress Enter to end your turn...");
    return true;
}

Game::MoveResult Game::playNormalCard(Player& current, std::size_t index) {
    Card card = current.hand()[index];
    std::vector<Position> open = board_.openPositionsFor(card);
    std::string status;

    while (true) {
        clearScreen();
        showState(current, open);
        if (!status.empty()) {
            out_ << "\n" << status << "\n";
            status.clear();
        }

        out_ << "\nPlaying " << card << ". Open spaces (marked > <):";
        for (std::size_t i = 0; i < open.size(); ++i) {
            out_ << "  " << (i + 1) << ") " << toString(open[i]);
        }
        out_ << "\n";

        std::string line = prompt("Choose a space by number or coordinate, or 'b' to go back: ");
        if (isBack(line)) {
            return MoveResult::Cancelled;
        }

        Position pos{0, 0};
        std::size_t choice = 0;
        if (parseIndex(line, open.size(), choice)) {
            pos = open[choice];
        } else if (!parsePosition(line, pos) || !contains(open, pos)) {
            status = "'" + line + "' isn't an open space for " + card.toString() + ".";
            continue;
        }

        placeChip(current, index, pos);
        return MoveResult::Played;
    }
}

Game::MoveResult Game::playTwoEyedJack(Player& current, std::size_t index) {
    Card card = current.hand()[index];
    std::string status;

    while (true) {
        clearScreen();
        showState(current);
        if (!status.empty()) {
            out_ << "\n" << status << "\n";
            status.clear();
        }

        out_ << "\nPlaying " << card << " (two-eyed jack): place a chip on ANY open space.\n";
        std::string line = prompt("Enter a coordinate (e.g. E5), or 'b' to go back: ");
        if (isBack(line)) {
            return MoveResult::Cancelled;
        }

        Position pos{0, 0};
        if (!parsePosition(line, pos)) {
            status = "'" + line + "' isn't a coordinate. Use a column letter and row number, e.g. E5.";
            continue;
        }
        if (!board_.canPlaceWild(pos)) {
            status = toString(pos) + " isn't open (it's taken or a free corner).";
            continue;
        }

        placeChip(current, index, pos);
        return MoveResult::Played;
    }
}

Game::MoveResult Game::playOneEyedJack(Player& current, std::size_t index) {
    Card card = current.hand()[index];
    std::vector<Position> removable = board_.removablePositions(current.id());
    if (removable.empty()) {
        // Nothing to remove; let the player pick another card
        clearScreen();
        showState(current);
        waitForEnter("\nThere are no opponent chips you can remove right now. "
                     "Press Enter to pick another card...");
        return MoveResult::Cancelled;
    }

    std::string status;
    while (true) {
        clearScreen();
        showState(current, removable);
        if (!status.empty()) {
            out_ << "\n" << status << "\n";
            status.clear();
        }

        out_ << "\nPlaying " << card << " (one-eyed jack): remove an opponent's chip.\n"
             << "Removable chips are marked > < (chips in a completed sequence are safe).\n";
        std::string line = prompt("Enter a coordinate, or 'b' to go back: ");
        if (isBack(line)) {
            return MoveResult::Cancelled;
        }

        Position pos{0, 0};
        if (!parsePosition(line, pos)) {
            status = "'" + line + "' isn't a coordinate. Use a column letter and row number, e.g. E5.";
            continue;
        }
        if (!board_.canRemove(pos, current.id())) {
            status = toString(pos) + " doesn't have an opponent chip you can remove.";
            continue;
        }

        board_.remove(pos);
        replaceCard(current, index);
        lastMove_ = current.name() + " played " + card.toString() +
                    " and removed the chip on " + toString(pos) + ".";
        lastPos_ = pos;
        hasLastMove_ = true;
        return MoveResult::Played;
    }
}

void Game::placeChip(Player& current, std::size_t index, const Position& pos) {
    Card card = current.hand()[index];
    board_.place(pos, current.id());
    int newSequences = board_.claimSequences(pos, current.id());
    current.addSequences(newSequences);
    replaceCard(current, index);

    lastMove_ = current.name() + " played " + card.toString() + " on " + toString(pos) + ".";
    if (newSequences > 0) {
        lastMove_ += "\n*** SEQUENCE! *** " + current.name() + " now has " +
                     std::to_string(current.sequences()) + " of " +
                     std::to_string(SEQUENCES_TO_WIN) + ".";
    }
    lastPos_ = pos;
    hasLastMove_ = true;
}

void Game::replaceCard(Player& current, std::size_t index) {
    deck_.discard(current.removeCard(index));
    Card drawn{Suit::Hearts, Rank::Two};
    if (deck_.draw(drawn)) {
        current.addCard(drawn);
    }
}

bool Game::discardDeadCard(Player& current, std::size_t index, std::string& status) {
    Card card = current.hand()[index];
    if (!board_.isDeadCard(card)) {
        status = card.toString() + " isn't dead: it still has an open space on the board.";
        return false;
    }
    replaceCard(current, index);
    status = "Swapped dead card " + card.toString() + " for " +
             current.hand().back().toString() + ". Now play a card.";
    return true;
}

bool Game::hasPlayableCard(const Player& current) const {
    for (const Card& card : current.hand()) {
        if (card.isOneEyedJack()) {
            if (!board_.removablePositions(current.id()).empty()) {
                return true;
            }
        } else if (card.isTwoEyedJack() || !board_.isDeadCard(card)) {
            // A two-eyed jack is always playable unless the board is full,
            // which can't happen before someone wins
            return true;
        }
    }
    return false;
}

void Game::showState(const Player& current, const std::vector<Position>& highlights) const {
    const Player& p1 = players_[0];
    const Player& p2 = players_[1];
    out_ << "SEQUENCE   " << p1.name() << " (" << p1.chip() << "): " << p1.sequences()
         << "/" << SEQUENCES_TO_WIN << " seq   " << p2.name() << " (" << p2.chip()
         << "): " << p2.sequences() << "/" << SEQUENCES_TO_WIN << " seq   Deck: "
         << deck_.size() << "\n\n";

    std::vector<Position> lit = highlights;
    if (lit.empty() && hasLastMove_) {
        lit.push_back(lastPos_);
    }
    board_.print(out_, lit);

    out_ << "Key: ** free corner (counts for both)   (X)/(O) chip   "
            "[X]/[O] chip in a sequence\n";
    if (!lastMove_.empty()) {
        out_ << "\nLast move: " << lastMove_ << "\n";
    }
    out_ << "\n";
    showHand(current);
}

void Game::showHand(const Player& current) const {
    out_ << current.name() << "'s hand (" << current.chip() << "):\n";
    const std::vector<Card>& hand = current.hand();
    for (std::size_t i = 0; i < hand.size(); ++i) {
        const Card& card = hand[i];
        std::string name = card.toString();
        out_ << "  " << (i + 1) << ") " << name << std::string(5 - name.size(), ' ');

        if (card.isTwoEyedJack()) {
            out_ << "two-eyed jack: wild, place a chip on any open space";
        } else if (card.isOneEyedJack()) {
            out_ << "one-eyed jack: remove an opponent's chip";
        } else {
            std::vector<Position> open = board_.openPositionsFor(card);
            if (open.empty()) {
                out_ << "DEAD (both spaces taken) - swap with 'd " << (i + 1) << "'";
            } else {
                out_ << "open: ";
                for (std::size_t j = 0; j < open.size(); ++j) {
                    out_ << (j > 0 ? ", " : "") << toString(open[j]);
                }
            }
        }
        out_ << "\n";
    }
}

void Game::showHelp() const {
    out_ << "HOW TO PLAY SEQUENCE (2 players)\n"
         << "--------------------------------\n"
         << "* Each player holds " << HAND_SIZE << " cards. On your turn, play one card and\n"
         << "  put a chip on a board space showing that card. You then draw a new card.\n"
         << "* Every card (except jacks) appears twice on the board.\n"
         << "* Two-eyed jacks (JD, JC) are wild: place a chip on any open space.\n"
         << "* One-eyed jacks (JH, JS) remove one opponent chip from the board.\n"
         << "  Chips that are part of a completed sequence can't be removed.\n"
         << "* A sequence is 5 of your chips in a row: across, down, or diagonal.\n"
         << "  The four corners (**) are free spaces that count for everyone.\n"
         << "  Your second sequence may share one chip with your first.\n"
         << "* First to " << SEQUENCES_TO_WIN << " sequences wins.\n"
         << "* Dead card: if both spaces for a card are covered, you may swap it\n"
         << "  for a new card ('d <number>') once per turn, then play.\n"
         << "\nCOORDINATES: column letter + row number, e.g. A1 (top-left) or J10.\n";
}

std::string Game::prompt(const std::string& message) {
    out_ << message << std::flush;
    std::string line;
    if (!std::getline(in_, line)) {
        throw InputClosed{};
    }
    return trim(line);
}

void Game::waitForEnter(const std::string& message) {
    prompt(message);
}

void Game::clearScreen() const {
    // Plain newlines work in every terminal, and push the previous
    // player's hand out of view
    out_ << std::string(60, '\n');
}
