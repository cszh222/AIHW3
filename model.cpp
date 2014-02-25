#include "model.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

Model::Model(string filename){
	ifstream myFile;
	string line;
	bool sizeSet = false;
	int vars, clauses;
	myFile.open(filename.c_str());

	while(!myFile.fail() && !sizeSet){
		myFile >> line;
		if(line == "c"){	
			getline(myFile, line);
		}
		else if(line == "p"){
			myFile >> line; //get cnf
			myFile >> vars; //variable count
			myFile >> clauses; //clauses count
			getline(myFile, line); //read rest of line
			sizeSet = true;
		}
	}

	this->mVariables = new vector<bool>(vars);

	int varInClause;
	stringstream lineStream;

	for(int i=0; i<clauses; i++){
		mClauses.push_back(map<int, bool>());
		//read each line
		getline(myFile, line);
		lineStream << line;
		lineStream >> varInClause;
		while(!lineStream.fail()){
			if(varInClause != 0){
				mClauses[i].insert(pair<int, bool>(varInClause, false));
			}
			lineStream >> varInClause;
		}
		lineStream.clear();
	}
}

Model::~Model(){
	delete mVariables;
	for(int i=0; i< mClauses.size(); i++){
		mClauses[i].clear();
	}
	mClauses.clear();
}

vector<bool>* Model::getVariables(){
	return mVariables;
}

vector< map<int, bool> >* Model::getClauses(){
	return &mClauses;
}

void Model::printVariables(){
	for(int i=0; i < mVariables->size(); i++){
		cout << ((*mVariables)[i] ? "1" : "0") << " ";
	}
	cout << endl;
}