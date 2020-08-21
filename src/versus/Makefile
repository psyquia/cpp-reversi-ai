CXX=g++ -O3 -fopenmp
OBJS = main.o board.o state.o state_mod.o myhelper.o ai.o ai_mod.o

all: reversi

reversi: $(OBJS)
	$(CXX) -o reversi $(OBJS)

main.o: main.cpp definitions.h
	$(CXX) -c main.cpp

ai.o: ai.cpp ai.h board.h state.h definitions.h
	$(CXX) -c ai.cpp

ai_mod.o: ai_mod.cpp ai_mod.h board.h state_mod.h definitions.h
	$(CXX) -c ai_mod.cpp

state.o: state.cpp state.h board.h
	$(CXX) -c state.cpp

state_mod.o: state_mod.cpp state_mod.h board.h
	$(CXX) -c state_mod.cpp

board.o: board.cpp board.h myhelper.h
	$(CXX) -c board.cpp

myhelper.o: myhelper.cpp myhelper.h
	$(CXX) -c myhelper.cpp

clean:
	$(RM) $(OBJS) reversi

.PHONY: all test clean

