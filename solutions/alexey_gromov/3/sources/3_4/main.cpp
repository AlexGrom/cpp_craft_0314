
#include <iostream>
#include <boost/strong_typedef.hpp>
#include <fstream>
#include <iostream>
#include <stock_data.h>
#include <market_message.h>
#include <vector>
#include <algorithm>
#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>


using namespace std;

const boost::uint32_t kTimeStep = 2;


typedef enum
{
    MARKET_OPEN     = 1,
    TRADE           = 2,
    QUOTE           = 3,
    MARKET_CLOSE    = 4

} messageType;


void generate_input_files()
{
	ifstream inputFile("input.txt", ios::in | ios::binary);

	for( size_t i = 1; i < 1000; i++ )
 	{
		ostringstream string_stream;
 		string_stream << "input_";
 		string_stream << fixed << setw( 3 ) << setfill( '0' );
 		string_stream << i << ".txt";
 		string input_filename = string_stream.str();
	    ofstream outputFile(input_filename, ios::out | ios::binary );
	    outputFile<<inputFile.rdbuf();
		inputFile.seekg(0);
	}
}

void processing_file(const int thread_index)
{
	ostringstream string_stream;

 	string_stream << "input_";
 	string_stream << fixed << setw( 3 ) << setfill( '0' );
 	string_stream << thread_index << ".txt";
 	string input_filename = string_stream.str();
 	string output_filename = boost::replace_all_copy( input_filename, "input", "output" );

	string output_path= BINARY_DIR + std::string( "/" ) + output_filename;
 	ofstream outputFile( output_path, ios::out | ios::binary );
	
	string input_path = BINARY_DIR + string( "/" ) + input_filename;
 	ifstream inputFile( input_path, ios::in | ios::binary );
	
	boost::int32_t maxTime = 0;

	while (!inputFile.eof())
    {
        binary_reader::market_message marketMessage(inputFile);
        
		if( inputFile.eof())
 		{
 			break;
 		}

        maxTime = max(maxTime, marketMessage.time());
        
        if(marketMessage.type() > MARKET_OPEN && marketMessage.type() < MARKET_CLOSE && marketMessage.time() + kTimeStep > maxTime)
        {
            marketMessage.write(outputFile);
        }
    }
	
	inputFile.close();
	outputFile.close();
 }


int main(int argc, const char * argv[])
{
	//generate_input_files();

	boost::thread_group threadpool;

	for( size_t i = 1; i < 1000; i++ )
 	{	
 		boost::thread *t = new boost::thread(processing_file, i );
 		t->start_thread();
 		threadpool.add_thread(t);
 	}
 
 	threadpool.join_all();
    return 0;
}

