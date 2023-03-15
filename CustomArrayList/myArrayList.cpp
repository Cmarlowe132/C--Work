#include "myArrayList.h"
#include <iostream>

using namespace std; 

/**
 * @brief Default constructor of the array list
 */
template <class T> 
myArrayList<T>::myArrayList() {
    size = 0; 
    data = NULL; 
}

/**
 * @brief Creates an ArrayList with the passed in size and array
 * 
 * @param num int, size
 * @param word T, array of the objects
 */
template <class T> 
myArrayList<T>::myArrayList(int const num, const T word[]) {
    size = num; 
    data = new T[size];
    for(int i = 0; i < num; i++) {
        data[i] = word[i];
    }
}

/**
 * @brief Copy constructor
 * @param rhs, myArrayList<T>, array list to be copied
 */
template <class T>
myArrayList<T>::myArrayList(const myArrayList<T> &rhs) {
    data = new T[rhs.size];
    for(int i = 0; i < rhs.size; i++) {
        data[i] = rhs.data[i];
    }
    size = rhs.size; 
}

/**
 * @brief array list destructor
 * 
 */
template <class T>
myArrayList<T>::~myArrayList() {
    delete [] data;
}

/**
 * @brief returns the size of the array
 * 
 * @return const int, size of the array
 */
template <class T> 
const int myArrayList<T>::length() const {
    return size;
}

/**
 * @brief empties the array list
 */
template <class T> 
void myArrayList<T>::clear() {
    delete [] data; 
    size = 0; 
}

/**
 * @brief Tests if the array list is empty
 * 
 * @return true, array list is empty
 * @return false, array list is no empty
 */
template <class T> 
const bool myArrayList<T>::isEmpty() const {
    if (size == 0) {
        return true; 
    }
    return false; 
}

/**
 * @brief Assignment operator overloader for the array list
 * @param rhs, ArrayList that is being set equal to
 * @return const myArrayList<T>&, original arraylist passed along for continuation
 */
template <class T> 
const myArrayList<T> & myArrayList<T>::operator=(const myArrayList<T> &rhs) {
    delete [] data; 
    size = rhs.size;
    data = new T [size];
    for(int i = 0; i < size; i++) {
        data[i] = rhs[i];
    }
    return rhs; 
}

/**
 * @brief Tests arraylists for equivalence
 * 
 * @param rhs, Array list being tested for equivalence
 * @return true, array lists are equal
 * @return false, arrays lists are not equal
 */
template <class T> 
const bool myArrayList<T>::operator==(const myArrayList<T> &rhs) const {
    if(size != rhs.size) {
        return false; 
    }
    for (int i = 0; i < size; i++) {
        if(rhs.data[i] != data[i])
            return false; 
    }
    return true; 
}

/**
 * @brief tests arraylists for being not equals
 * 
 * @param rhs array list being tested for non equivalence
 * @return true, array lists are not equal
 * @return false, array lists are equal
 */
template <class T> 
const bool myArrayList<T>::operator!=(const myArrayList<T> &rhs) const {
    if(size != rhs.size) {
        return true; 
    }
    for (int i = 0; i < size; i++) {
        if(rhs.data[i] != data[i])
            return true; 
    }
    return false; 
}

/**
 * @brief Addition operator overloader
 * 
 * @param rhs Array list being added onto the end of the array list
 * @return const myArrayList<T>, Arraylist to be passed along
 */
template <class T>
const myArrayList<T> myArrayList<T>::operator+(const myArrayList<T> &rhs) const {
    T temp[size+rhs.size];
    myArrayList<T> temporary(size+rhs.size, temp);
    for (int i = 0; i < size; i++) {
        temporary.data[i] = data[i];
    }

    for (int i = 0; i < rhs.size; i++) {
        temporary.data[i+size] = rhs.data[i];
    }    
    return temporary; 
}

/**
 * @brief Subraction operator overloader, subtracts one list from the other if the array list is found within the other 
 * 
 * @param rhs array list to be subtracted
 * @return const myArrayList<T>, Array list to be passed along
 */
template <class T>
const myArrayList<T> myArrayList<T>::operator-(const myArrayList<T> &rhs) const {
    myArrayList<T> tempList(*this);
    int startIndex = tempList.find(rhs);
    if(startIndex == -1) {
        return *this; 
    }
    T temp[size - rhs.size];
    int counter = 0; 
    for(int i = 0; i < size; i++) {
        if(i < startIndex || i >= (startIndex+rhs.size)) {
            temp[counter] = data[i];
            counter++;
        }
    } 
    myArrayList<T> temporary((size-rhs.size), temp);
    return temporary;
}

/**
 * @brief Bracket operator for insertion
 * 
 * @param num Index for the object to be inserted
 * @return T& Object that will be changed
 */
template <class T> 
T& myArrayList<T>::operator[](const int num) {
    if(num >= 0 && num < size) {
        return data[num];
    }
    else {
        cout << "Index out of bounds, exiting the program" << endl; 
        exit(0);
    }
}

/**
 * @brief Bracket operator for returning the object at the index
 * 
 * @param num Index to be returned
 * @return const T object being returned
 */
template <class T> 
const T myArrayList<T>::operator[](const int num) const {
    if(num < 0 || num >= size) {
        cout << "Index out of bounds, exiting the program" << endl;
        exit(0);
    }
    return data[num];
}

/**
 * @brief Overloads the stream extraction operator
 * 
 * @param lhs in stream being taken from
 * @param rhs arraylist being extracted into
 * @return istream& passed on for cascading
 */
template <class U>
istream & operator>>(istream &lhs, myArrayList<U> &rhs) {
    cout << "Enter " << rhs.size << " number of items";
    for(int i = 0; i < rhs.size; i++) {
        lhs >> rhs[i];
    } 
    return lhs;
}

/**
 * @brief Stream insertion operator overload 
 * 
 * @param lhs ostream being being added to
 * @param rhs array list being inserted
 * @return ostream& passed along for cascadability
 */
template<class U> 
ostream & operator<<(ostream &lhs, myArrayList<U> &rhs) {
    for(int i = 0; i < rhs.length(); i++) {
        lhs << rhs[i];
    }
    return lhs; 
}

/**
 * @brief Finds if on array list is within another
 * 
 * @param rhs list being found within the other
 * @return const int index that the arraylist within the other starts at
 */
template <class T> 
const int myArrayList<T>::find(const myArrayList<T>& rhs) const {
    if (rhs.size > size || rhs.size == 0) {
        return(-1); 
    }

    for(int i = 0; i < size; i++) {
        bool ifContains = false; 
        if (data[i] == rhs.data[0] && rhs.size+i < size) {
            ifContains = true;
            for (int k = 0; k < rhs.size; k++) {
                if(data[k+i] != rhs.data[k]) {
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