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

void checkEarth(vector<string> &data, size_t i, size_t j)
{
    if	((i>0) && data[i-1][j] == 'o')
	{
		data[i-1][j] ='~';

		checkEarth(data,i-1,j);
	}
	
	if ((i<data.size()-1) && data[i+1][j] == 'o')
	{
        data[i+1][j] ='~';

        checkEarth(data,i+1,j);
	}

    if	((j<data[0].length()-1) && data[i][j+1] == 'o')
	{
        data[i][j+1] = '~';

		checkEarth(data,i,j+1);
    
	}

	if  ((j > 0) && data[i][j-1] == 'o')
	{
        data[i][j-1] = '~';

		checkEarth(data,i,j-1);
	}

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

	vector<string> data;

	while(!inputFile.eof())
	{
      string str;
	  getline(inputFile, str);
	  data.push_back(str);
	}
    
	inputFile.close();

	size_t islandsCount = 0;

	for (size_t i=0;i<data.size();i++)
	{   
		for (size_t j = 0;j<data[i].length();j++)
		{
			if (data[i][j] == 'o')
			{
				checkEarth(data,i, j);
				islandsCount++;
			}
		}
	}
       
	ofstream outputFile(BINARY_DIR"/output.txt", ios::app);

	if (!outputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		getch();
		exit(1);
	}
    outputFile<<islandsCount;
	
	outputFile.close();

	getch();

	return 0;
}
