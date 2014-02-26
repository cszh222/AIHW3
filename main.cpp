#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include "model.h"
#include "walksat.h"

using namespace std;

int getdir(string dir, vector<string> &files);

int main(){
	//seed random
	srand(time(NULL));
	//get file names
	string dir = "A3Formulas";
	vector<string> files;
	getdir(dir, files);

	//file to write to for data
	ofstream outFile("walkSat2.csv", ofstream::trunc);
	outFile <<"File, Satisfied, Time, Clauses\n";

	int solved = 0;
	//solve for each file
	for(int i=0; i<files.size(); i++){
		if(files[i] != "." && files[i]!=".."){
			for(int j=0; j<10; j++){
				cout <<files[i] <<endl;
				outFile << files[i] << ", ";
				Model *myModel = new Model(dir+"/"+files[i]);
				Walksat *myWalksat = new Walksat(myModel);

				clock_t begin = clock();//start timer
				if(myWalksat->solve(50, 10000)){
					outFile <<"SAT, ";
					solved++;
				}
				else {
					outFile <<"UNSAT, ";
				}
				outFile << double(clock() - begin)/CLOCKS_PER_SEC <<"seconds, ";
				outFile << myWalksat->getMaxSat() <<endl;;
				delete myWalksat;
			}			
		}
	}
	outFile.close();
	cout <<"Solved: " <<solved <<endl;

	return 0;
}

//function from: http://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/
/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}