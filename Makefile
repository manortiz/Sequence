TARGETS  = card 			# temporary

DBGFLAGS = -gdwarf-4		# Used for debugging-related flags (-g, etc.)
OPTFLAGS = 

CXX      = g++
CXXFLAGS = -gdwarf-4 -Wall -Wextra -pedantic -std=c++17

all: $(TARGETS)

card: card.o 
	$(CXX) $(CXXFLAGS) -o card card.o

card.o: card.cpp card.hpp
	$(CXX) $(CXXFLAGS) -c card.cpp

clean:
	rm -f *.o $(TARGETS)
