// Laba_1.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace std;


string prepareString(string& str)
{
	char chars[] = " \-";

    for (size_t i = 0; i < strlen(chars); ++i)
	{
          str.erase (remove(str.begin(), str.end(), chars[i]), str.end());
	}

	transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

int main(int argc, char* argv[])
{
	
	ifstream   inputFile(BINARY_DIR"/input.txt");
	if (!inputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		getch();
		exit(1);
	}

	ofstream outputFile(BINARY_DIR"/output.txt", ios::app);

	if (!outputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		getch();
		exit(1);
	}
	
	
	string text;
	getline(inputFile, text);
    text= prepareString(text);

    while(!inputFile.eof())
	{
		string str;
		getline(inputFile, str);

        reverse(str.begin(), str.end());
		str = prepareString(str);
        
		size_t found = text.find(str);

        if (found!= string::npos)
		{
              outputFile<<"YES"<<endl;
        }
		else
		{
              outputFile<<"NO"<<endl;
		}
	 }
    inputFile.close();
	outputFile.close();

	getch();
	return 0;
}

