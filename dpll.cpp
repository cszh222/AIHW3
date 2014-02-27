#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <iostream>
#include "model.h"
#include "dpll.h"

using namespace std;

Dpll::Dpll(Model* model){
	mModel = model;
	
	mMaxSat = 0;
}

Dpll::~Dpll(){
	delete mModel;
}

void Dpll::assignVars(){
	map<int, bool*>::iterator it;
	for(int i=0; i<mModel->mClauses.size(); i++){
		for(it=mModel->mClauses[i].begin(); it!= mModel->mClauses[i].end(); it++){
			if((*mModel->mVariables)[abs(it->first)-1] == NULL){
				if(it->second != NULL){
					delete it->second;
				}
				it->second = NULL;
			}
			else{
				if(it->second != NULL){
					delete it->second;
				}
				bool* litVal = new bool;
				*litVal = *((*mModel->mVariables)[abs(it->first)-1]);
				if(it->first < 0){
					*litVal = !*((*mModel->mVariables)[abs(it->first)-1]);
					it->second = litVal;
				}
				else{
					*litVal = *((*mModel->mVariables)[abs(it->first)-1]);
					it->second = litVal;
				}
			}//outer if/else
		}//inner loop
	}//outer loop
}

//returns -1 for false, 1 for true 0 for when there is unassigned literal
short Dpll::checkClauseSatisfied(int clause){
	map<int, bool*>::iterator it;
	//assume it is false
	short trueFound = -1;
	for(it = mModel->mClauses[clause].begin(); it != mModel->mClauses[clause].end(); it++){
		if(it->second == NULL){
			//there is an unassigned value, so it can never be false
			trueFound = 0;
		}
		else{
			if(it->first > 0 && *(it->second) != *(*mModel->mVariables)[it->first-1])
				cerr <<"something happened2\n";
			if(it->first < 0 && *(it->second) == *(*mModel->mVariables)[abs(it->first)-1])
				cerr <<"something happened3\n";
			if(*(it->second)){
				trueFound = 1;
				break;
			}
		}	
	}
	return trueFound;
}

//returns true if there is atleast 1 false clause
bool Dpll::falseFound(){
	bool falseFound = false;
	for(int i=0; i < mModel->mClauses.size(); i++){
		if(checkClauseSatisfied(i) == (short)-1){
			falseFound = true;
		}
	}
	return falseFound;
}

//returns number of clauses satisfied
int Dpll::checkSatisfied(){
	int clauseCount = 0;
	for(int i=0; i < mModel->mClauses.size(); i++){
		if(checkClauseSatisfied(i) == (short)1){
			clauseCount++;
		}
	}
	return clauseCount;
}

bool Dpll::solve(){
	//assign all variables to clauses, this assumes variables assigned through recursing from earlier calls
	assignVars();
	
	int clausesSat = checkSatisfied();

	if(clausesSat > mMaxSat){
		mMaxSat = clausesSat;
	}
	//early termination
	if(clausesSat == mModel->mClauses.size()){
		//assign false to rest of the unassigned variables
		for(int i=0; i< mModel->mVariables->size(); i++){
			if((*mModel->mVariables)[i] == NULL){
				(*mModel->mVariables)[i] = new bool;
				*(*mModel->mVariables)[i] = false;
			}			
		}
		//assign variables and do one last check for sanity
		assignVars();
		if(checkSatisfied() != mModel->mClauses.size()){
			cerr <<"somethind bad happened\n";
		}
		return true;
	}

	if(falseFound()){
		return false;
	}

	//find pure symbols
	map<int, bool> pureSymbols;
	findPure(pureSymbols);
	if(pureSymbols.size() != 0){
		//pure symbols found, assign them
		map<int, bool>::iterator it;
		bool *val;
		for(it = pureSymbols.begin(); it!=pureSymbols.end(); it++){
			val = new bool;
			(*val) = it->second;
			(*mModel->mVariables)[abs(it->first)-1] = val;
		}
		//recurse
		if(!solve()){
			//solving failed, undo assignments
			for(it = pureSymbols.begin(); it!=pureSymbols.end(); it++){
				delete (*mModel->mVariables)[abs(it->first)-1];
				(*mModel->mVariables)[abs(it->first)-1] = NULL;
			}
			pureSymbols.clear();
			return false;
		}
		else{
			return true;
		}
	}

	//find unit clauses
	map<int, bool>unitSymbols;
	findUnit(unitSymbols);
	if(unitSymbols.size() != 0){
		//unit symbols found, assign them
		map<int, bool>::iterator it;
		bool *val;
		for(it = unitSymbols.begin(); it!=unitSymbols.end(); it++){
			val = new bool;
			(*val) = it->second;
			(*mModel->mVariables)[abs(it->first)-1] = val;
		}
		//recurse
		if(!solve()){
			//solving failed, undo assignments
			for(it = unitSymbols.begin(); it!=unitSymbols.end(); it++){
				delete (*mModel->mVariables)[abs(it->first)-1];
				(*mModel->mVariables)[abs(it->first)-1] = NULL;
							cerr <<"herrrrrrrrrrre"<<endl;
			}
			unitSymbols.clear();
			return false;
		}
		else{
			return true;
		}
	}

	//pick first unassigned variable and do dfs
	int varToAssign;
	for(int i=0; i<mModel->mVariables->size(); i++){
		if((*mModel->mVariables)[i] == NULL){
			varToAssign = i;
		}
	}
	
	(*mModel->mVariables)[varToAssign] = new bool;

	//assign true
	*(*mModel->mVariables)[varToAssign] = true;
	if(solve()){
		return true;
	}
	//true didn't work, try false
	*(*mModel->mVariables)[varToAssign] = false;
	if(solve()){
		return true;
	}
	//false didnt work, undo assignment and return false
	delete (*mModel->mVariables)[varToAssign];
	(*mModel->mVariables)[varToAssign] = NULL;
	return false;
}

void Dpll::findPure(map<int, bool> &pureSymbols){
	map<int, bool> symbolOccurances;

	map<int, bool*>::iterator it;
	for(int i=0; i<mModel->mClauses.size(); i++){
		if(checkClauseSatisfied(i)==0){
			//clause has unassigned literal
			for(it=mModel->mClauses[i].begin(); it!=mModel->mClauses[i].end(); it++){
				//insert into symbol occurance
				if(symbolOccurances.find(it->first) == symbolOccurances.end()){
					//current element not in occurance, add to it
					//let it be false, only the key matters
					symbolOccurances[it->first] = false;
				}
			}
		}
	}

	//i+1 needed to accomondate variable index starting with 0
	//check the variables and remove non pures
	for(int i=0; i<mModel->mVariables->size(); i++){
		if( (*mModel->mVariables)[i] != NULL){
			//variable already has assignment, remove it from occurances
			if(symbolOccurances.find(i+1) != symbolOccurances.end()){
				symbolOccurances.erase(i+1);
			}
			//variable already has assignment, remove it from occurances
			if(symbolOccurances.find(-1*(i+1)) != symbolOccurances.end()){
				symbolOccurances.erase(-1*(i+1));
			}
		}
		else{
			if(symbolOccurances.find(i+1) != symbolOccurances.end() && symbolOccurances.find(-1*(i+1)) != symbolOccurances.end()){
				//the symbol is not pure, remove both
				symbolOccurances.erase(i+1);
				symbolOccurances.erase(-1*(i+1));
			}
		}
	}

	//remaining symbols are pure, place it in pureSymbols
	map<int, bool>::iterator it2;
	for(it2 = symbolOccurances.begin(); it2!=symbolOccurances.end(); it2++){
		if(it2->first > 0){
			pureSymbols[it2->first] = true;
		}
		else{
			pureSymbols[it2->first] = false;
		}
	}

}

void Dpll::findUnit(map<int, bool> &unitSymbols){
	map<int, bool> symbolOccurances;//unit symbols found in clauses
	map<int, bool> clauseOccurances;//occurance of an unassigned symbol in clause
	map<int, bool*>::iterator it;
	for(int i=0; i<mModel->mClauses.size(); i++){
		if(checkClauseSatisfied(i)==0){
			//clause has unassigned literal
			for(it=mModel->mClauses[i].begin(); it!=mModel->mClauses[i].end(); it++){
				if(it->second != NULL){
					//no value assigned to literal in clause, it is an occurance, only key matters, assign false
					clauseOccurances[it->first] = false;
				}
			}
			if(clauseOccurances.size() == 1){
				if(symbolOccurances.find(clauseOccurances.begin()->first) != symbolOccurances.end()){
					symbolOccurances[clauseOccurances.begin()->first] = false;
				}
			}
			clauseOccurances.clear();
		}
	}

	//symbolOccurances now has unit literals of a clause
	//check if there is already value assigned to the variable, 
	//and for occurances of positive and negative literal
	//i+1 needed to accomondate variable index starting with 0
	for(int i=0; i<mModel->mVariables->size(); i++){
		if( (*mModel->mVariables)[i] != NULL){
			//variable already has assignment, remove it from occurances
			if(symbolOccurances.find(i+1) != symbolOccurances.end()){
				symbolOccurances.erase(i+1);
			}
			//variable already has assignment, remove it from occurances
			if(symbolOccurances.find(-1*(i+1)) != symbolOccurances.end()){
				symbolOccurances.erase(-1*(i+1));
			}
		}
		else{
			if(symbolOccurances.find(i+1) != symbolOccurances.end() && symbolOccurances.find(-1*(i+1)) != symbolOccurances.end()){
				//the symbol is not pure, remove both
				symbolOccurances.erase(i+1);
				symbolOccurances.erase(-1*(i+1));
			}
		}
	}

	//remaining symbols are unit, place it in unitSymbols
	map<int, bool>::iterator it2;
	for(it2 = symbolOccurances.begin(); it2!=symbolOccurances.end(); it2++){
		if(it2->first > 0){
			unitSymbols[it2->first] = true;
		}
		else{
			unitSymbols[it2->first] = false;
		}
	}
}


void Dpll::printVars(){
	mModel->printVariables();
}

int Dpll::getMaxSat(){
	return mMaxSat;
}
