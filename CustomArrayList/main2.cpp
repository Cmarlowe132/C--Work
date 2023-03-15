#include <iostream>
#include "myArrayList.cpp"

using namespace std;

int main()
{
    myArrayList<char> mS1;
	char testString[] = "Test String";
	myArrayList<char> mS2(11, testString);
	myArrayList<char> mS3(mS2);
    char testAgain[] = "Testing";
    myArrayList<char> mS4(7, testAgain);

	cout << "Test of constructors 1+1+1 and print 1. " << endl;
	cout << mS1 << endl;
	cout << mS2 << endl;
	cout << mS3 << endl;

	mS1 = mS2 = mS3 = mS4;
	cout << "Test of cascaded assignment operator. " << endl;
	cout << mS1 << endl;
	cout << mS2 << endl;
	cout << mS3 << endl;


    cin >> mS2;
    cout << "The operator>>. +2 points" << endl;
	cout << mS2 << endl;
	if(mS1 != mS2)
		cout << "The operator != working. +1" << endl;
	else
		cout << "The operator != NOT working" << endl;
    
    mS3 = mS2;
    if(mS2 == mS3)
		cout << "The operator == working. +1" << endl;
	else
		cout << "The operator != NOT working" << endl;

	mS1 = mS4;
	cout << "Test of the operator =. mS1 should be the same as mS2. +2" << endl;
	cout << mS1 << endl;
	cout << mS4 << endl;


	cout << "The operator[] function should show T and n. +1 point" << endl;
	cout << mS1[0] << " " << mS1[5] << endl;

	cout << "The operator[] function should show J and J. +1 point" << endl;
	mS1[0] = 'J';
	mS1[5] = 'J';
	cout << mS1 << endl;

	cout << "The two strings before operator+" << endl;
	cout << mS2 << endl;
	cout << mS3 << endl;
	mS2 = mS2 + mS3 + mS3;
	cout << "The two strings after operator+. 2 points" << endl;
	cout << mS2 << endl;
	cout << mS3 << endl;

	cout << "The two strings before operator-" << endl;
	cout << mS2 << endl;
	cout << mS3 << endl;
	mS2 = mS2 - mS3;
	cout << "The two strings after operator-. 2 points" << endl;
	cout << mS2 << endl;
	cout << mS3 << endl;
    return 0;
}