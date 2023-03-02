#include <iostream> 
#include "String.h"

using namespace std;

int main(int argc, char **argv) {
    
    char word[] = "hello";
    String s1(word, 5);
    char word2[] = "hi";
    String s2(word2, 2);
   char word3[] = "l";
  String s3(word3, 1); 
    cout << "String length: "; 
    cout << s1.length() << endl; 
    s1.print();
    cout << "Is the String empty: ";
   cout << s1.empty() << endl;
   cout << "Character at index 2: ";
 cout << s1.at(2) << endl;    
    cout << "Where does l first occur: "; 
    cout << s1.find(s3, 0) << endl; ; 
    cout << "Do String 1 and String 2 equal: ";
    cout << s1.equal(s2)<< endl; 
    cout << "Adding s2 to s1" << endl;
    s1.add(s2); 
    s1.print();
    cout << "Number of times 'l' is in String: " << endl; 
    cout << s1.count(s3); 
    cout << "Emptying s1" << endl; 
    s1.clear(); 
    s1.print(); 
    cout << "Is s1 empty? : "; 
    cout << s1.empty(); 
    return(0);
}
