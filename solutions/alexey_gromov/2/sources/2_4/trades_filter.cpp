#include <iostream>
#include <boost/strong_typedef.hpp>
#include <fstream>
#include <iostream>
#include <stock_data.h>
#include <market_message.h>
#include <vector>
#include <algorithm>

using namespace std;

const boost::uint32_t kTimeStep = 2;


typedef enum
{
    MARKET_OPEN     = 1,
    TRADE           = 2,
    QUOTE           = 3,
    MARKET_CLOSE    = 4

} messageType;


int main(int argc, const char * argv[])
{
    ifstream inputFile(BINARY_DIR"/input.txt", ios::binary);
    
    if (!inputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		exit(1);
	}

    ofstream outputFile(BINARY_DIR"/output.txt", ios::binary);
    
    boost::uint32_t maxTime = 0;
    
    while (!inputFile.eof())
    {
        binary_reader::market_message marketMessage(inputFile);
        
        maxTime = max(maxTime, marketMessage.time());
        
        if(marketMessage.type() > MARKET_OPEN && marketMessage.type() < MARKET_CLOSE && marketMessage.time() + kTimeStep > maxTime)
        {
            marketMessage.write(outputFile);
        }
    }
    
    
    inputFile.close();
    outputFile.close();
    
    return 0;
}
 