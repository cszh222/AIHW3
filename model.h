#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Model{

public:
	vector<bool*> *mVariables;
	vector< map<int, bool*> > mClauses;
	
	Model(string filename);
	~Model();

	void printVariables();
};
#endif