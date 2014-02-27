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

	this->mVariables = new vector<bool*>(vars);

	//initialize to null
	for (int i=0; i< mVariables->size(); i++){
		(*mVariables)[i] = NULL;
	}

	int varInClause;
	stringstream lineStream;

	for(int i=0; i<clauses; i++){
		mClauses.push_back(map<int, bool*>());
		//read each line
		getline(myFile, line);
		lineStream << line;
		lineStream >> varInClause;
		while(!lineStream.fail()){
			if(varInClause != 0){
				//initialize to null
				mClauses[i].insert(pair<int, bool*>(varInClause, NULL));
			}
			lineStream >> varInClause;
		}
		lineStream.clear();
	}

	myFile.close();
}

Model::~Model(){
	for(int i=0; i<mVariables->size(); i++){
		if((*mVariables)[i] != NULL){
			delete (*mVariables)[i];
			(*mVariables)[i] = NULL;
		}
	}
	delete mVariables;

	map<int, bool*>::iterator it;
	for(int i=0; i< mClauses.size(); i++){
		for(it=mClauses[i].begin(); it!=mClauses[i].end(); it++){
			if(it->second != NULL){
				delete it->second;
				it->second = NULL;
			}
		}
		mClauses[i].clear();
	}
	mClauses.clear();
}

void Model::printVariables(){
	for(int i=0; i < mVariables->size(); i++){
		cout << ( *((*mVariables)[i]) ? (i+1) : -1*(i+1)) << " ";
	}
	cout << endl;
}