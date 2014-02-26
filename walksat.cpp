#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <iostream>
#include "model.h"
#include "walksat.h"

using namespace std;

Walksat::Walksat(Model* model){
	mModel = model;
	mClauseSatisfaction = new vector<bool>(mModel->mClauses.size());

	//initially assign all satisfaction to false
	for(int i=0; i<mClauseSatisfaction->size(); i++){
		(*mClauseSatisfaction)[i] = false;
	}
	mMaxSat = 0;
}

Walksat::~Walksat(){
	delete mClauseSatisfaction;
	delete mModel;
}

bool Walksat::solve(int prob, int maxFlips){
	//randomly assing values to variables
	for(int i = 0; i < mModel->mVariables->size(); i++ ){
		(*mModel->mVariables)[i] = rand()%2 == 1 ? true : false;
	}

	//check the number of satisfied clauses
	assignVars();
	checkSatisfied();
	bool solutionFound = false;

	vector<int> unsatisfiedClauses;
	for(int i=0; i<maxFlips; i++){
		unsatisfiedClauses.clear();
		unsatisfiedClauses = getUnsatClauses();
		if(unsatisfiedClauses.size() == 0){
			solutionFound = true;
			break;
		}
		int randClause = unsatisfiedClauses[(rand()%unsatisfiedClauses.size())];
		if(rand()%100 < prob){
			//randomly flip 1 variable in clause
			map<int, bool>::iterator it = mModel->mClauses[randClause].begin();
			advance(it, rand()%mModel->mClauses[randClause].size());
			//flip the var
			(*mModel->mVariables)[abs(it->first)-1] = !(*mModel->mVariables)[abs(it->first)-1];
		}
		else{
			map<int, bool>::iterator it;
			int varToFlip;
			int satClauses = 0;
			for(it=mModel->mClauses[randClause].begin(); it!=mModel->mClauses[randClause].end(); it++){
				//flip the variable
				(*mModel->mVariables)[abs(it->first)-1] = !(*mModel->mVariables)[abs(it->first)-1]; 
				 //assign to clauses
				assignVars();
				int curSat = checkSatisfied(); //get the current satisfied c
				if(curSat > satClauses){
					varToFlip = abs(it->first)-1;
					satClauses = curSat;
				}
				//flip it back
				(*mModel->mVariables)[abs(it->first)-1] = !(*mModel->mVariables)[abs(it->first)-1]; 
				//assign variables back
				assignVars(); 
			}
			//flip the var with most clause satisfaction
			(*mModel->mVariables)[varToFlip] = !(*mModel->mVariables)[varToFlip]; 
		}
		//assign the vars
		assignVars();
		//update the mSatisfaction vector

		if(checkSatisfied() > mMaxSat){
			mMaxSat = checkSatisfied();
		}
	}

	if(solutionFound){
		cout <<"SAT ";
		for(int i=0; i<mModel->mVariables->size(); i++){
			if(!(*mModel->mVariables)[i]){
				cout << -1*(i+1) << " ";
			}
			else{
				cout << (i+1) <<" ";
			}
		}
	}
	else{
		cout <<"UNSAT ";
	}
	cout <<endl;
	return solutionFound;
}

int Walksat::checkSatisfied(){
	int clauseCount = 0;
	for(int i=0; i<mClauseSatisfaction->size(); i++){
		if(checkClauseSatisfied(i)){
			(*mClauseSatisfaction)[i] = true;
			clauseCount++;
		}
		else{
			(*mClauseSatisfaction)[i] = false;
		}
	}
	return clauseCount;
}

bool Walksat::checkClauseSatisfied(int clause){
	map<int, bool>::iterator it;

	bool trueFound = false;
	for(it = mModel->mClauses[clause].begin(); it != mModel->mClauses[clause].end(); it++){
		if(it->first > 0 && it->second != (*mModel->mVariables)[it->first-1])
			cerr <<"something happened";
		if(it->first < 0 && it->second == (*mModel->mVariables)[abs(it->first)-1])
			cerr <<"something happened2";
		if(it->second){
			trueFound = true;
			break;
		}	
	}
	return trueFound;
}

void Walksat::assignVars(){
	map<int, bool>::iterator it;
	for(int i=0; i<mModel->mClauses.size(); i++){
		for(it=mModel->mClauses[i].begin(); it!= mModel->mClauses[i].end(); it++){
			if(it->first < 0){
				it->second = !(*mModel->mVariables)[abs(it->first)-1];
			}
			else
				it->second = (*mModel->mVariables)[it->first-1];
		}
	}
}

vector<int> Walksat::getUnsatClauses(){
	vector<int> unsatClauses;
	for(int i=0; i<mClauseSatisfaction->size(); i++){
		if(!(*mClauseSatisfaction)[i])
			unsatClauses.push_back(i);
	}
	return unsatClauses;
}

int Walksat::getMaxSat(){
	return mMaxSat;
}