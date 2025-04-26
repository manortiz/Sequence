TARGETS  = main	

DBGFLAGS = -gdwarf-4		# Used for debugging-related flags (-g, etc.)
OPTFLAGS = 

CXX      = g++
CXXFLAGS = -gdwarf-4 -Wall -Wextra -pedantic -std=c++17 -mconsole

all: $(TARGETS)

main: main.o card.o cell.o
	$(CXX) $(CXXFLAGS) -o main main.o card.o cell.o

main.o: main.cpp card.hpp cell.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

cell.o: cell.cpp cell.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c cell.cpp

card.o: card.cpp card.hpp
	$(CXX) $(CXXFLAGS) -c card.cpp

clean:
	rm -f *.o $(TARGETS)
