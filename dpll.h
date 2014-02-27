#ifndef DPLL_H
#define DPLL_H

#include <map>
#include <string>
#include <vector>
#include "model.h"

using namespace std;

class Dpll{

private:
	Model* mModel;
	int mMaxSat;

public:
	Dpll(Model* model);
	~Dpll();

	void assignVars();
	int checkSatisfied();
	bool falseFound();
	short checkClauseSatisfied(int clause);
	bool solve();
	void findPure(map<int, bool> &pureSymbols);
	void findUnit(map<int, bool> &unitSymbols);
	void printVars();
	int getMaxSat();

};


#endif