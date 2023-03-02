#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

vector<string> takeInput(string fileName); //takes in the data from the input file
void printToFile(int, string fileName); //outputs data to the output file
int HorspoolsMatching (string pattern, string text); //Horspool matching algorithm
int lookupCharNumber(char c); //finds the number for the character passed in that relates to the shift table;
void generateShiftTable(string pattern, int lookupTable[]); //generates the shift table

int main(int argc, char** argv) {
    vector<string> input = takeInput("input.txt"); //take in the input file
    int matchingIndex = HorspoolsMatching(input.at(0), input.at(1)); //run the search algorithm
    auto start = std::chrono::high_resolution_clock::now(); //starts clock for measuring time
    auto end = std::chrono::high_resolution_clock::now(); //end time for measuring algorithm time
    auto int_s = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start); // gets the time elapsed
    cout << "Elapsed run time is: " << int_s.count() << " nanoseconds" << endl; //prints out time elapsed
    printToFile(matchingIndex, "output.txt");
    return(0);
}

vector<string> takeInput(string fileName) {
  fstream workingFile(fileName);
  if(!workingFile.is_open()) {
    cout << "Invalid File" << endl;
    exit(0);
  }
  vector<string> returnString;
  int count = 0;
  std::string line;
  while(getline(workingFile, line)) {
      returnString.push_back(line);
  }
  workingFile.close();
  return returnString;
}


void printToFile(int outputNum, string fileName){ 
    ofstream outfile;
    outfile.open(fileName, ofstream::out | ofstream::trunc);
    if(!outfile.is_open()) {
        cout << "Invalid File" << endl;
        exit(0);
    }
    outfile << outputNum;
    outfile.close();
}

int HorspoolsMatching (string pattern, string text) {
    const int lookupTableSize = 27;
    int lookupTable[lookupTableSize]; //Create the lookup table
    for(int i = 0; i < lookupTableSize; i++) { //fill the lookup table with the length of the pattern string
        lookupTable[i] = pattern.length();
    }
    generateShiftTable(pattern, lookupTable); //generate the shift table
    int endIndex = pattern.length()-1;
    while (endIndex <= text.length()-1) {
        int k = 0;
        while (k <= pattern.length()-1 && pattern.at(pattern.length()-1-k) == text.at(endIndex - k)) { //go through the pattern and find how much it matches
            k++;
        }
        if (k==pattern.length()) { //the string matches if you get to this point
            return endIndex-pattern.length()+1;
        }
        else {
            endIndex += lookupTable[lookupCharNumber(text.at(endIndex))]; // shift by the amount in the lookup table
        }
    }
    return -1;
}

int lookupCharNumber(char c) { //Returns the lookup value of the character passed in
    char a = 'a';
    int num = c % a;
    if (num < 0 || num > 25) {
        num = 26;
    }
    return num;
}

void generateShiftTable(string pattern, int lookupTable[]) { //generates the shift table by figuring out how far each char in the pattern is from the end.
    for(int i =0; i < pattern.length()-1; i++) {
        lookupTable[lookupCharNumber(pattern.at(i))] = pattern.length()-1-i;
    }
}

