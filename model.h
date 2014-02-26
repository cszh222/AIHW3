#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Model{

//just make the members public for easy access
//no need for getter and setter
public:
	vector<bool> *mVariables;
	vector< map<int, bool> > mClauses;
	
	Model(string filename);
	~Model();

	void printVariables();
};
#endif