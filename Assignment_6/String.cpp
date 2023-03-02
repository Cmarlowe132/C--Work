#include "String.h"
#include <iostream>

using namespace std; 

String::String() {
    size = 0; 
    word = new char[size];
}

String::String(char characters[], const int _size) {
    size = _size; 
    word = new char[size];
    for(int i = 0; i < size; i++) {
        word[i] = characters[i];
    }
}

String::String(const String &rhs) {
    size = rhs.size;
    word = new char[size];
    for (int i = 0; i < size; i++) {
        word[i] = rhs.word[i];
    }
}

String::~String() {
    delete [] word;
}

int String::length() const {
    return size; 
}

void String::clear() {
    delete [] word;
    size = 0;
    word = new char[size];
}

void String::print() const {
    for(int i = 0; i < size; i++) {
        cout << word[i];
    }
    cout << endl; 
}

bool String::equal(const String &rhs) const {
    if(size != rhs.size)
        return false; 
    for(int i = 0; i < size; i++) {
        if(word[i] != rhs.word[i])
            return false; 
    }
    return true; 
}

char String::at(const int num) const {
    if (num < 0 || num >= size) {
        cout << "Number is outside the range of the String" << endl; 
        cout << "Please enter a number between 0 and " << size-1 << endl;
        return(0);
    }
    return word[num];
}

int String::find(const String& rhs, const int startIndex) const {
    if (rhs.size > size || rhs.size == 0) {
        cout << "Substring too large or does not exist" << endl; 
        exit(0); 
    }

    for(int i = 0; i < size; i++) {
        bool ifContains = false; 
        if (word[i+startIndex] == rhs.word[0] && rhs.size+i+startIndex <= size+1) {
            ifContains = true;
            for (int k = 0; k < rhs.size; k++) {
                if(word[k+i+startIndex] != rhs.word[k]) {
                    ifContains = false; 
                    break; 
                }
            }
        }
        if(ifContains == true) {
            return i; 
        }
    }
    cout << "String not found" << endl; 
        return -1; 
}

bool String::add(const String &rhs) {
    char *temp = new char [size+rhs.size];
    for (int i = 0; i < size; i++) {
        temp[i] = word[i];
    }
    for (int i = 0; i < rhs.size; i++) {
        temp[i+size] = rhs.word[i];
    }

    delete [] word;
    word = new char [size+rhs.size];
    size = size+rhs.size;
    for(int i =0; i<size; i++) {
        word[i] = temp[i];
    }
    delete [] temp; 
    return true; 
}

int String::count(const String &rhs) const {
    if(size < rhs.size)
        return 0;
    bool ifContains = false;
    int counter = 0;
    for(int i = 0; i < size; i++) {
        if (word[i] == rhs.word[0] && rhs.size+i <= size+1) {
            ifContains = true;
            for (int k = 0; k < rhs.size; k++) {
                if(word[k+i] != rhs.word[k]) {
                    ifContains = false; 
                    break; 
                }
            }
        }
        if(ifContains) 
            counter++;
        ifContains = false;
    }
    return counter;    
}

bool String::empty() const {
    return (size==0);
}