#include <iostream>
#include "model.h"
#include <vector>

int main(){
	Model *myModel = new Model("fixedDimacs/10.40.160707067.cnf");

	myModel->printVariables();
	vector<bool> *myBool = myModel->getVariables();

	for(int i=0; i< myBool->size(); i++){
		if( i%2 == 0)
			(*myBool)[i] = false;
		else
			(*myBool)[i] = true;
	}

	myModel->printVariables();

	delete myModel;

	return 0;
}