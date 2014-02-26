#ifndef WALKSAT_H
#define WALKSAT_H

#include <map>
#include <string>
#include <vector>
#include "model.h"

using namespace std;

class Walksat{

private:
	vector<bool> *mClauseSatisfaction;
	Model* mModel;
	int mMaxSat;
public:
	Walksat(Model* model);
	~Walksat();

	bool solve(int prob, int maxFlips);
	int checkSatisfied();
	bool checkClauseSatisfied(int clause);
	void assignVars();
	vector<int> getUnsatClauses();
	int getMaxSat();

};


#endif