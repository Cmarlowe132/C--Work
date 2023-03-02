#ifndef MYSUDOKU_H
#define MYSUDOKU_H
#include <iostream>
#include <fstream>
#include<string>

using namespace std; 

class mySudoku {
    public:
        mySudoku();
        mySudoku(string);
        ~mySudoku(); 

        void ReadData(string);
        void PrintData();
        bool CheckColumn(int, int);
        bool CheckRow(int, int);
        bool CheckSubSquare(int, int, int);
        bool SolvePuzzle();
    private:
        int sudokuBoard[9][9];
};

#endif // MYSUDOKU_H