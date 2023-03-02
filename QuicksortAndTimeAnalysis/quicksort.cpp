#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std; 
void takeInput(vector<float>&, string); //Function to take in numbers from a file, all numbers are stored in the float bector
void quickSort(vector<float>&, int, int); //Main Quicksort function, calls partition and then divides the vector into the two pieces
int partition(vector<float>&, int, int); // Partition function, finds where the pivot goes and ensures that the left of the pivot is less than the pivot and right of the pivot is greater than the pivot
void printToFile(vector<float>, string); //Prints the organized float vector to the passed in file
void swap(vector<float>&, int, int); //Swap helper function which swaps the two passed in points in the float vector
bool checkIfSolutionCorrect(vector<float>); //Helper function that checks if the vector has actually been sorted properly

int main(int argc, char *argv[]) {
    vector<float> arr; //vector that will contain all of the points for quicksort
    string file = "input.txt"; //input file
    string outfile = "output.txt"; //output file
    takeInput(arr, file); //takes in the input from the input file
    auto start = std::chrono::high_resolution_clock::now(); //finds the time for the start of the function
    quickSort(arr, 0, arr.size()-1);//runs the quick sort algorithm
    auto end = std::chrono::high_resolution_clock::now();//finds the time that the algorithm ended at
    auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start);//calculates the time elapsed in microseconds for the function
    cout << "Elapsed run time is: " << int_s.count() << " microseconds" << endl; //Prints out the time elapsed in microseconds
    printToFile(arr, outfile); //prints the organized array to the output file
    cout << "Solution is correct: " << checkIfSolutionCorrect(arr) << endl; //prints whether or not the vector is sorted, 0 if not, 1 if it is
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

void quickSort(vector<float> &arr, int l, int r) {
    if(l < r) {
        int s = partition(arr, l, r); //Find the pivot and sort the array into larger and smaller sides
        quickSort(arr, l, s-1);//run quicksort on the left side of the partition
        quickSort(arr, s+1, r);//run quicksort of the right side of the partition

    }
}

int partition(vector<float> &arr, int l, int r) {
    float pivot = arr.at(l);
    int i = l+1; 
    int j = r; 
    while (true) {
        while(i < r && arr[i] <= pivot) { //Go until i is at the right of the array, or the current value at i is greater than the pivot
            i++;
        }
        while(i > l && arr[j] > pivot) { //Go until j is at the left of the array, or the current value at j is less than or equal to the pivot
            j--;
        }
        swap(arr, i, j); //swap whats at i and j
        if(i >= j) {
            break;
        }
    } 
    swap(arr, i, j); //undo last swap
    swap(arr, l, j);//swap the pivot location with j
    return j;
} 

void printToFile(vector<float> arr, string fileName){ 
    ofstream outfile;
    outfile.open(fileName, ofstream::out | ofstream::trunc);
    if(!outfile.is_open()) {
        cout << "Invalid File" << endl;
        exit(0);
    }

    for(int i = 0; i < arr.size(); i++) {
        outfile<<arr.at(i) << " "; 
    }
    outfile.close();
}


void swap(vector<float> &arr, int spot1, int spot2) {
    float temp = arr.at(spot2);
    arr.at(spot2) = arr.at(spot1);
    arr.at(spot1) = temp;
}

bool checkIfSolutionCorrect(vector<float> arr) {
    for(int i = 0; i < arr.size()-1; i++) {
        if(arr[i] > arr[i+1]) {
            return false;
        }
    }
    return true; 
}