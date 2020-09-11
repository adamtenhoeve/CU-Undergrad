CXX = g++
CXXFLAGS	= -std=c++11 -Wall

all: ElectoralMap.o TextUI.o Election.o main

clean:
	rm main ElectoralMap.o Election.o TextUI.o

main: main.cpp ElectoralMap.o Election.o TextUI.o
	$(CXX) $(CXXFLAGS) main.cpp ElectoralMap.o Election.o TextUI.o -o main

TextUI.o:
	$(CXX) $(CXXFLAGS) -c TextUI.cpp

Election.o:
	$(CXX) $(CXXFLAGS) -c Election.cpp

ElectoralMap.o: 
	$(CXX) $(CXXFLAGS) -c ElectoralMap.cpp