#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std; 
void takeInput(vector<float>&, string); //Function to take in numbers from a file, all numbers are stored in the float bector
void printToFile(vector<vector<float>>, string, int); //Prints the matrix to the output file
void fillMatrix(vector<vector<float>> &matrix, int sideLength, vector<float> originalArr); //Filles the matrix from the vector that comes in from the input file
vector<vector<float>> Floyds(vector<vector<float>> arr, int sideLength); //Floyd's Algorithm
float findMin(float num1, float num2);//Finds the minimum between two numbers

int main(int argc, char *argv[]) {
    vector<float> arr; //vector that will contain all of the matrix points
    string file = "input.txt"; //input file
    string outfile = "output.txt"; //output file
    takeInput(arr, file); //takes in the input from the input file
    int sideLength = sqrt(arr.size()); //Finds the sidelength of the matrix
    vector<vector<float>> matrix(sideLength); //Matrix that will be populated
    fillMatrix(matrix, sideLength, arr); //Matrix is filled
    vector<vector<float>> output = Floyds(matrix, sideLength); //Runs Floyd's algorithm and takes the output matrix
    printToFile(output, outfile, sideLength); //prints the matrix to the output file
    return 0;
}

void takeInput(vector<float> &arr, string fileName) {
  fstream workingFile(fileName);
  if(!workingFile.is_open()) {
    cout << "Invalid File" << endl;
    exit(0);
  }
  float num;
  while(workingFile >> num) {
      arr.push_back(num);
  }
  workingFile.close();
}

void printToFile(vector<vector<float>> arr, string fileName, int sideLength){ 
    ofstream outfile;
    outfile.open(fileName, ofstream::out | ofstream::trunc);
    if(!outfile.is_open()) {
        cout << "Invalid File" << endl;
        exit(0);
    }
    for(int j = 0; j < sideLength; j++) {
        for(int i = 0; i < sideLength; i++) {
            outfile<<arr[j][i] << " "; 
        }
        outfile << '\n';
    }

    outfile.close();
}

void fillMatrix(vector<vector<float>> &matrix, int sideLength, vector<float> originalArr) {
    int count = 0;
    for(int i = 0; i < sideLength; i++) {
        for(int j = 0; j < sideLength; j++) {
            matrix[i].push_back(originalArr[count]);
            count++;
        }
    }
}

vector<vector<float>> Floyds(vector<vector<float>> arr, int sideLength) {
    vector<vector<float>> outputArr(sideLength);
    for(int i = 0; i < sideLength; i++) {
        for(int j = 0; j < sideLength; j++) {
            outputArr[i].push_back(arr[i][j]);
        }
    }
    for(int k = 0; k < sideLength; k++) {
        for(int i = 0; i < sideLength; i++) {
            for(int j = 0; j < sideLength; j++) {
                outputArr[i][j] = findMin(outputArr[i][j], outputArr[i][k] + outputArr[k][j]);
            }
        }
    }
    return outputArr;
}

float findMin(float num1, float num2) {
    if(num1 > num2) {
        return num2;
    }
    return num1;
}