TARGETS  = sequence

CXX      = g++
CXXFLAGS = -gdwarf-4 -Wall -Wextra -pedantic -std=c++17

OBJS     = main.o game.o board.o deck.o player.o cell.o card.o

all: $(TARGETS)

sequence: $(OBJS)
	$(CXX) $(CXXFLAGS) -o sequence $(OBJS)

main.o: main.cpp game.hpp board.hpp cell.hpp card.hpp deck.hpp player.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

game.o: game.cpp game.hpp board.hpp cell.hpp card.hpp deck.hpp player.hpp
	$(CXX) $(CXXFLAGS) -c game.cpp

board.o: board.cpp board.hpp cell.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c board.cpp

deck.o: deck.cpp deck.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c deck.cpp

player.o: player.cpp player.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c player.cpp

cell.o: cell.cpp cell.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c cell.cpp

card.o: card.cpp card.hpp
	$(CXX) $(CXXFLAGS) -c card.cpp

clean:
	rm -f *.o $(TARGETS) $(TARGETS).exe
