GPP = g++
CXXFLAGS = -g

all: DPLL

model.o: model.h model.cpp
	$(GPP) $(CXXFLAGS) -c model.cpp -o model.o

dpll.o: dpll.h dpll.cpp
	$(GPP) $(CXXFLAGS) -c dpll.cpp -o dpll.o

main.o: main.cpp
	$(GPP) $(CXXFLAGS) -c main.cpp -o main.o

DPLL: model.o main.o dpll.o
	$(GPP) $(CXXFLAGS) main.o dpll.o model.o -o DPLL

clean: 
	rm -rf DPLL *.o
