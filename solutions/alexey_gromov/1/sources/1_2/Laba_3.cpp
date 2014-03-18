// Laba_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	ifstream   inputFile(BINARY_DIR"/Input.txt");
	if (!inputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		getch();
		exit(1);
	}

    size_t numbersCount;
	inputFile>>numbersCount;

	vector<double> rationalNumbers;

	for (size_t i=0;i<numbersCount;i++)
	{
       double number;

       inputFile>>number;

	   rationalNumbers.push_back(number);
	}

	vector<double> codesNumbers;

	while(!inputFile.eof())
	{
       double number;

       inputFile>>number;

	   codesNumbers.push_back(number);
	}
	inputFile.close();

    ofstream outputFile(BINARY_DIR"/Output.txt", ios::app);

	if (!outputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		getch();
		exit(1);
	}

    for (i=0;i<codesNumbers.size();i++)
	{
		string check = "NO";

		for(size_t j=0;j<rationalNumbers.size();j++)
		{
           if( (int)(codesNumbers[i]*1E+4) == (int)(rationalNumbers[j]*1E+4))
		   {
			   check="YES";
		   }
		}
		outputFile<<check<<endl;
	}
	
	outputFile.close();

	getch();

	return 0;
}
