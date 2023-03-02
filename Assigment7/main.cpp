#include <iostream> 
#include "String.h"

using namespace std;

int main(int argc, char **argv) {  
  char word[] = "hello";
  String s1(word, 5);
  cout << "First String: " << s1 << endl;
  char word2[] = "iamaword";
  String s2(word2, 8);
  cout << "Second String: " << s2 << endl;
  char word3[] = "hel";
  String s3(word3, 3); 
  cout << "Third String: " << s3 << endl;
  String s4(s1);
  cout << "Fourth String, copy of first: " << s4 << endl;
  cout << "Adding String 1 and String 2: " << s1 + s2 << endl;
  s3 = s1-s3; 
  cout << "Subtracting String 3 from String 1: " << s3 << endl; 
  cout << "Testing String 1 equal to String 4: " << (s1 == s4) << endl << "Testing String 1 Equal to String 3: " << (s1 == s3) << endl;
  cout << "Testing String 1 not equal to String 4: " << (s1 != s4) << endl << "Testing String 1 not equal to String 3: " <<  (s1 != s3) << endl; 
  s2[4] = s4[4];

  cout << "Replacing the 4th index of String 2 with the 4th index of String 4: " << s2 << endl;
  cout << "Preincrementing String 1: " << ++s1 << endl << "Post incrementing String1: " << s1++ << endl;
  cout << "Taking input from the console into String 3" << endl;
  cin >> s3; 
  cout << s3 << endl;
  return(0);
}
