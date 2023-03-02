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

const int String::length() const {
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
    if (rhs.size > size || rhs.size == 0 || startIndex < 0 || startIndex >= size) {
        cout << "Substring too large or does not exist, or index out of range" << endl; 
        exit(0); 
    }

    for(int i = 0; i < size; i++) {
        bool ifContains = false; 
        if (word[i+startIndex] == rhs.word[0] && rhs.size+i+startIndex <= size) {
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

char String::getChar(const int index) const {
    if(index < 0 || index >= size) {
        cout << "Index out of range" << endl;
        exit(1);
    }
    return word[index];
}

void String::setChar(const int index, const char character) {
    if(index < 0 || index >= size) {
        cout << "Index out of range" << endl;
        exit(1);
    }
    word[index] = character;
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
    word = temp;
    size = size+rhs.size;
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

const bool String::empty() const {
    return (size==0);
}

/**
 * Overloads the + operator, appending a String onto another String and returning the newly created String
 * Does not modify either String
 * @param rhs String being appended to the other String
 * @return const String Returned String that is appended
 */
const String String::operator+(const String &rhs) const {
    char tempWord[rhs.size+size];
    String temp(tempWord, rhs.size+size); 
    for(int i =0; i < size; i++) {
        temp.word[i] = word[i];
    }
    for(int i=0; i < rhs.size; i++) {
        temp.word[i+size] = rhs.word[i];
    }
    return temp;
}

/**
 * Equals operator overload, tests if two Strings are equal
 * 
 * @param rhs String being tested for equality
 * @return true Strings are equal
 * @return false Strings are not equal
 */
const bool String::operator==(const String &rhs) const {
    if (size != rhs.size) {
        return false; 
    }
    for(int i = 0; i < size; i++) {
        if(word[i]!=rhs.word[i]) {
            return false; 
        }
    }
    return true;
}

/**
 * Overloads the assignment operator, sets the left String to the passed in String
 * 
 * @param rhs String to be set equal to
 * @return const String& String being returned for cascading
 */
const String & String::operator=(const String &rhs) {
    delete [] word;
    size = rhs.size;
    word = new char[size];
    for(int i = 0; i < size; i++) {
        word[i] = rhs.word[i];
    }
    return rhs;
}

/**
 * Overloads the not equals operator
 * 
 * @param rhs String to test for not equalness
 * @return true Strings are not equal
 * @return false Strings are equal
 */
const bool String::operator!=(const String &rhs) const{
    if (size != rhs.size) {
        return true; 
    }
    for(int i = 0; i < size; i++) {
        if(word[i]!=rhs.word[i]) {
            return true; 
        }
    }
    return false;
}

/**
 * Overloads the minus operator
 * Returns a String with the passed in String removed from the calling String if the passed in String is found, Returns the original if not found
 * @param rhs String to be subtracted
 * @return const String that has had the passed in String removed from it
 */
const String String::operator-(const String &rhs) const{
    String tempStr(*this);
    int index = tempStr.find(rhs, 0);
    if(index == -1) {
        return *this; 
    }
    char temp[rhs.size]; 
    int counter = 0;
    for(int i = 0; i < size; i++) {
        if(i < index || i >= index + rhs.size) {
            temp[counter] = word[i];
            counter++;
        }
    }
    String tempString(temp, (size-rhs.size));
    return tempString;
}

/**
 * Overloads the brackets operator for accessing a value
 * Returns a value at the passed in index
 * @param index int, index to be retrieved
 * @return const char at the index
 */
const char String::operator[](const int index) const {
    if(index < 0 || index >= size) {
        cout << "Index out of bounds";
        exit(0);
    }
    return word[index];
}

/**
 * Overloads the brackets operator for changing a value
 * Returns a value at an index to be changed
 * @param index int, index to be changed
 * @return char& Char being changed
 */
char& String::operator[](const int index) {
    if(index < 0 || index >= size) {
        cout << "Index out of bounds";
        exit(0);
    }
    return word[index];
}

/**
 * Overloads the post increment operator, adds a blank space to the end of the String and returns the original String
 * 
 * @return const String Original String
 */
const String String::operator++(const int dummy) {
    String temp(*this);
    char* tempChar = new char[size+1];
    for(int i = 0; i < size; i++) {
        tempChar[i] = word[i];
    }
    tempChar[size] = ' ';
    delete [] word;
    word = tempChar;
    size++;
    return temp; 
}

/**
 * Preincrement operator, adds a blank space to the end of the String
 * 
 * @return const String& String with the space on the end of it
 */
const String & String::operator++() {
    char* tempChar = new char[size+1];
    for(int i = 0; i < size; i++) {
        tempChar[i] = word[i];
    }
    tempChar[size] = ' ';
    delete [] word;
    word = tempChar;
    size++;
    return *this; 
}

/**
 * Overloads the in-stream Stream insertion operator to insert incoming chars from the console into a String
 * 
 * @param lhs In stream to have chars taken from
 * @param rhs String to have its characters changed
 * @return istream& for cascadability
 */
istream & operator>>(istream &lhs, String &rhs) {
    cout << "Enter " << rhs.length() << " characters" << endl; 
    for(int i = 0; i < rhs.length(); i++) {
        lhs >> rhs[i];
    }
    return lhs;
}

/**
 * Overloads the out-stream extraction operator to take the chars out from a String into the console
 * 
 * @param lhs outstream to be modified 
 * @param rhs String to be extracted to the out stream
 * @return ostream& out-stream to be output
 */
ostream & operator<<(ostream &lhs, const String &rhs) {
    for (int i = 0; i < rhs.length(); i++) {
        lhs << rhs[i];
    }
    return lhs;
}