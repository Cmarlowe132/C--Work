#ifndef STRING_H
#define STRING_H
#include <iostream>
/**
 * @author Carter Marlowe
 * 11/10/21 @7:15
 * This program performs the functions of Strings and extends into overloading operators
 * To create Strings, a character array has to be first created and its size must be known, and then the String constructor can be called with these two passed in
 * The program can print out the contents of a String or use cout to print out Strings
 * 
 */

using namespace std; 

class String {
        friend istream & operator>>(istream &lhs, String &rhs);
        friend ostream & operator<<(ostream &lhs, const String &rhs);
    
    public: 
        String(); 
        String(char[], const int);
        String(const String &);
        ~String(); 
       
        const int length() const; 
        void clear();
        const bool empty() const;
        char at(const int) const;
        int find(const String&, const int) const;
        bool equal(const String &) const;
        bool add(const String &);
        void print() const;
        int count(const String &) const; 
        char getChar(const int) const;
        void setChar(const int, const char);
        const String operator+(const String &) const;
        const bool operator==(const String &) const;
        const String & operator=(const String &);
        const bool operator!=(const String &) const;
        const String operator-(const String &) const;
        const char operator[](const int) const;
        char & operator[](const int);
        const String operator++(const int);
        const String & operator++();

    protected: 

    private: 
        int size; 
        char *word; 
};

#endif // STRING_H