#include <iostream>
#include "mySudoku.h"
#include <string>

using namespace std;

int main(int argc, char **argv) {

        if(argc != 2) {
                cout << "This program requires 1 input file to read a sudoku puzzle from, please only include 1 file: " << endl;
                exit(0); 
        }
        string fileName = argv[1];
        cout << fileName << endl; 
        
        mySudoku S1(fileName);


        S1.PrintData();
        cout << endl;
        S1.SolvePuzzle(); 
        S1.PrintData(); 

        return(0);
}