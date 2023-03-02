#include "mySudoku.h"

mySudoku::mySudoku()
{
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            sudokuBoard[i][j] = 0;
        }
    }
    //ctor
}

mySudoku::mySudoku(string fileName)
{
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) { 
            sudokuBoard[i][j] = 0;
        }
    }
    ReadData(fileName);
}

mySudoku::~mySudoku()
{
    //dtor
}

void mySudoku::ReadData(string fileName) {
    ifstream input_file(fileName);
    int num;

	if(!input_file.is_open()) {
		cout << "Error opening the file " << fileName << " \n";
		exit(1);
	}
        int counter(0);
        int counter2(0);
		while(input_file >> num) {
                sudokuBoard[counter][counter2] = num;
            counter2++;
            if (counter2 == 9) {
                counter2 = 0; 
                counter++;
            }
	}

	input_file.close();
}

void mySudoku::PrintData() {
    for(int i = 0; i < 9; i++) {
        for(int k =0; k < 9; k++) {
            cout << sudokuBoard[i][k] << " ";
        }
        cout << endl;
    }
}



bool mySudoku::CheckColumn(int column, int num) {
    for(int i = 0; i < 9; i++) {
        if(sudokuBoard[i][column] == num)
            return true;
    }
    return false;
}

bool mySudoku::CheckRow(int row, int num) {
    for(int i = 0; i < 9; i++) {
        if(sudokuBoard[row][i] == num)
            return true;
    }
    return false;
}

bool mySudoku::CheckSubSquare(int row, int column, int num) {
    int startColumn = (column / 3) * 3;
    int startRow = (row / 3) * 3;

    for(int i = startRow; i < startRow+3; i++) {
        for(int j = startColumn; j < startColumn+3; j++) {
            if(sudokuBoard[i][j] == num)
                return true;
        }
    }
    return false;
}

bool mySudoku::SolvePuzzle() {
    bool hasChanged = true;
    while (hasChanged) {
        hasChanged = false;
        for(int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if(sudokuBoard[i][j] == 0){
                    int possibleNum = 0;
                    int numOfPossibilities = 0;
                    for(int k = 1; k < 10; k++){
                        if(!CheckRow(i, k) && !CheckColumn(j, k) && !CheckSubSquare(i, j, k)) {
                            possibleNum = k;
                            numOfPossibilities++;
                        }
                    }
                    if(numOfPossibilities == 1) {
                        sudokuBoard[i][j] = possibleNum;
                        hasChanged = true;
                    }
                }
            }
        }
    }
    for(int i = 0; i < 9; i++) {
        for(int k = 0; k < 9; k++) {
            if(sudokuBoard[i][k] == 0)
                return false;
        }
    }
    return true;
}