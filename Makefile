GPP = g++
CXXFLAGS = -g

all: walkSat

model.o: model.h model.cpp
	$(GPP) $(CXXFLAGS) -c model.cpp -o model.o

main.o: main.cpp
	$(GPP) $(CXXFLAGS) -c main.cpp -o main.o

walkSat: model.o main.o
	$(GPP) $(CXXFLAGS) main.o model.o -o walkSat

clean: 
	rm -rf walkSat main.o model.o
