#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

using namespace std; 
void takeInput(vector<float>&, string);//Function which takes in numbers from the passed in file and populates the vector of floats.
void quickSort(vector<float>&, int, int);//Function which runs the quicksort algorithm, calls the partition and separates into two quicksorts on either side of the pivot
int partition(vector<float>&, int, int);//Calculates the pivot location and ensures that everything to the left of the pivot is smaller than the pivot, and everything to the right is greater
void generateRandomNumbersToFile(int, string); //Generates a number of random numbers based on the passed in int, and stores them in the passed in file.
void swap(vector<float>&, int, int);//Helper method for quicksort to swap two values at the int locations in the float vector
void run10Points(vector<int> &, string); //Function to run quicksort 100 times with vectors of 10 values, and calculate the total time in the passed in vector
void run100Points(vector<int> &, string); //Function to run quicksort 100 times with vectors of 100 values, and calculate the total time in the passed in vector
void run1000Points(vector<int> &, string); //Function to run quicksort 100 times with vectors of 1000 values, and calculate the total time in the passed in vector
void run10000Points(vector<int> &, string); //Function to run quicksort 100 times with vectors of 10000 values, and calculate the total time in the passed in vector
void run100000Points(vector<int> &, string); //Function to run quicksort 100 times with vectors of 100000 values, and calculate the total time in the passed in vector

int main(int argc, char *argv[]) {
    string file = "input.txt";//Input file
    vector<int> times;//Vector of the total times of each test
    run10Points(times, file);//tests quicksort with 10 inputs
    run100Points(times, file);//tests quicksort with 100 inputs
    run1000Points(times, file);//tests quicksort with 1000 inputs
    run10000Points(times, file);//tests quicksort with 10000 inputs
    run100000Points(times, file);//tests quicksort with 100000 inputs
    vector<double> averages;//vector to find the average times
    averages.push_back(times[0] / 100.0);//average time of test 1
    averages.push_back(times[1] / 100.0);//average time of test 2
    averages.push_back(times[2] / 100.0);//average time of test 3
    averages.push_back(times[3] / 100.0);//average time of test 4
    averages.push_back(times[4] / 100.0);//average time of test 5
    cout << "Average time for 10 points:  " << averages[0] << " microseconds" <<endl;//prints off average of test 1
    cout << "Average time for 100 points:  " << averages[1] << " microseconds" <<endl;//prints off average of test 2
    cout << "Average time for 1000 points:  " << averages[2] << " microseconds" <<endl;//prints off average of test 3
    cout << "Average time for 10000 points:  " << averages[3] << " microseconds" <<endl;//prints off average of test 4
    cout << "Average time for 100000 points:  " << averages[4] << " microseconds" <<endl;//prints off average of test 5
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

void swap(vector<float> &arr, int spot1, int spot2) {
    float temp = arr.at(spot2);
    arr.at(spot2) = arr.at(spot1);
    arr.at(spot1) = temp;
}

void run10Points(vector<int> &times, string file) {
    times.push_back(0);
    for(int i = 0; i < 100; i++) {
        vector<float> arr;
        generateRandomNumbersToFile(10, file);
        takeInput(arr, file);
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size()-1);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        int time = int_s.count();
        times[times.size()-1] += time;
    }
}

void run100Points(vector<int> &times, string file) {
    times.push_back(0);
    for(int i = 0; i < 100; i++) {
        vector<float> arr;
        generateRandomNumbersToFile(100, file);
        takeInput(arr, file);
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size()-1);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        int time = int_s.count();
        times[times.size()-1] += time;
    }
}

void run1000Points(vector<int> &times, string file) {
    times.push_back(0);
    for(int i = 0; i < 100; i++) {
        vector<float> arr;
        generateRandomNumbersToFile(1000, file);
        takeInput(arr, file);
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size()-1);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        int time = int_s.count();
        times[times.size()-1] += time;
    }
}

void run10000Points(vector<int> &times, string file) {
    times.push_back(0);
    for(int i = 0; i < 100; i++) {
        vector<float> arr;
        generateRandomNumbersToFile(10000, file);
        takeInput(arr, file);
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size()-1);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        int time = int_s.count();
        times[times.size()-1] += time;
    }
}

void run100000Points(vector<int> &times, string file) {
    times.push_back(0);
    for(int i = 0; i < 100; i++) {
        vector<float> arr; 
        generateRandomNumbersToFile(100000, file);
        takeInput(arr, file);
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size()-1);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        int time = int_s.count();
        times[times.size()-1] += time;
    }
}
