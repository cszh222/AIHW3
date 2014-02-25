#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Model{

private:
	vector<bool> *mVariables;
	vector< map<int, bool> > mClauses;

public: 
	
	Model(string filename);
	~Model();

	vector<bool>* getVariables();
	vector< map<int, bool> >* getClauses();

	void printVariables();
};
#endif