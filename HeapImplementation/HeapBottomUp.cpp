#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

void takeInput(vector<int> &arr, string fileName); //Function for taking in input numbers from a file, populates arr with integer values from file with name fileName
void heapBottomUp(vector<int> &heap); //Performs the heapBottomUp algorithm to construct a heap
void printToFile(vector<int> arr, string fileName); //Prints the resulting heap to an output file
void generateRandomNumbersToFile(int inputSize, string file);


int main(int argc, char** argv) {
    vector<int> arr; //Vector that will hold the heap
    arr.push_back(0); //pushes back the vector by one so heap operations can be performed easily
    //generateRandomNumbersToFile(10, "input.txt");
    takeInput(arr, "input.txt"); // Takes input from input file named input.txt
    auto start = std::chrono::high_resolution_clock::now(); //starts clock for measuring time
    heapBottomUp(arr); //Performs the bottom up heap construction
    auto end = std::chrono::high_resolution_clock::now(); //end time for measuring algorithm time
    auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start); // gets the time elapsed
    cout << "Elapsed run time is: " << int_s.count() << " microseconds" << endl; //prints out time elapsed
    printToFile(arr, "output.txt"); //sends the output heap to a file named output.txt
    return(0);
}

void takeInput(vector<int> &arr, string fileName) {
  fstream workingFile(fileName);
  if(!workingFile.is_open()) {
    cout << "Invalid File" << endl;
    exit(0);
  }
  int num;
  int count = 0;
  while(workingFile >> num) {
      if(count == 0) {
          count++;
      }
      else {
         arr.push_back(num); 
      }
  }
  workingFile.close();
}

void heapBottomUp(vector<int> &heap) {    
    int n = heap.size()-1;
    for(int i = n/2; i > 0; i--) { //Starts at first parent and will move up the heap
        int k = i;
        int v = heap.at(i);
        bool isHeap = false;
        while(!isHeap && 2*k <= n) { //checks if children are greater than their parent, if yes the parent is swapped by its largest child
            int j = 2*k;
            if(j < n) {
                if(heap.at(j) < heap.at(j+1)) {
                    j++;
                }
            }
            if(v >= heap.at(j)) {
                isHeap = true; 
            }
            else {
                heap.at(k) = heap.at(j);
                k = j;
            }
        }
        heap.at(k) = v; //Places the original value where the lowest spot it can go
    }
}


void printToFile(vector<int> arr, string fileName){ 
    ofstream outfile;
    outfile.open(fileName, ofstream::out | ofstream::trunc);
    if(!outfile.is_open()) {
        cout << "Invalid File" << endl;
        exit(0);
    }

    for(int i = 1; i < arr.size(); i++) {
        outfile<<arr.at(i) << " "; 
    }
    outfile.close();
}

void generateRandomNumbersToFile(int inputSize, string file) {
    ofstream outfile;
    outfile.open(file, ofstream::out | ofstream::trunc);
    srand((unsigned) time(0));
    if(!outfile.is_open()) {
        cout << "Invalid File" << endl;
        exit(0);
    }

    for(int i = 0; i < inputSize; i++) {
        outfile<< rand()%200000 -100000 << " ";
    }
    outfile.close();
}
