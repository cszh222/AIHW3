GPP = g++
CXXFLAGS = -g -O3

all: walkSat

model.o: model.h model.cpp
	$(GPP) $(CXXFLAGS) -c model.cpp -o model.o

walksat.o: walksat.h walksat.cpp
	$(GPP) $(CXXFLAGS) -c walksat.cpp -o walksat.o

main.o: main.cpp
	$(GPP) $(CXXFLAGS) -c main.cpp -o main.o

walkSat: model.o main.o walksat.o
	$(GPP) $(CXXFLAGS) main.o walksat.o model.o -o walkSat

clean: 
	rm -rf walkSat *.o
