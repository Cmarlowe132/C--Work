/**
 * @file myArrayList.h
 * @author Carter Marlowe
 * @brief Creates a template class that functions similarly to the ArrayList in Java
 * @date 2021-11-28
 * I wrote my own main function to test all of the operators
 */
#ifndef MYARRAYLIST_H
#define MYARRAYLIST_H
#include <iostream>

using namespace std;

template <class T>
class myArrayList {
        template <class U>
        friend istream & operator>>(istream &lhs, myArrayList<U> &rhs);
        template <class U> 
        friend ostream & operator<<(ostream &lhs, const myArrayList<U> &rhs);
    public:
        myArrayList();
        myArrayList(const int, const T *);
        myArrayList(const myArrayList<T> &);
        ~myArrayList();

        const int length() const; 
        void clear(); 
        const bool isEmpty() const; 
        const int find(const myArrayList<T>& rhs) const;

        const myArrayList<T> & operator=(const myArrayList<T> &);
        const bool operator==(const myArrayList<T> &) const; 
        const bool operator!=(const myArrayList<T> &) const; 
        const myArrayList<T> operator+(const myArrayList<T> &) const; 
        const myArrayList<T> operator-(const myArrayList<T> &) const; 
        T& operator[](const int); 
        const T operator[](const int) const; 


    
    private: 
        T *data; 
        int size; 

};

#endif // MYARRAYLIST_H