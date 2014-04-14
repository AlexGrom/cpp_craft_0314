

#include <iostream>
#include <fstream>
#include <market_message.h>
#include <vector>
#include <algorithm>
#include <boost/strong_typedef.hpp>
#include <map>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>


using namespace std;

const int kBufferSize = 2048;


struct messages_counts_data
{
    messages_counts_data(): messages_count( 0 ), time_count( 0 ), cur_time(0)
    {
    };
    
    uint32_t messages_count;
    uint32_t time_count;
    uint32_t cur_time;
};


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
	 if (!inputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		exit(1);
	}
	boost::uint32_t current_time = 0;
    
    map<uint32_t, uint32_t> message_type_sizes;
    map<uint32_t, uint32_t> message_type_counts;
    map<uint32_t, messages_counts_data> message_type_data;
    
    while (!inputFile.eof())
    {
        binary_reader::market_message market_message(inputFile);
        
        if (inputFile.eof())
        {
            break;
        }
        
        boost::uint32_t  message_size = sizeof(boost::uint32_t) * 3 + market_message.len() * sizeof(char);
        
        if( message_type_sizes[ market_message.type()] + message_size > kBufferSize )
		{
			continue;
		}
        
        message_type_counts[market_message.type()]++;
		message_type_sizes[market_message.type()] += message_size;
        message_type_data[market_message.type()].messages_count ++;
        
		if( market_message.time() != current_time )
		{
			message_type_sizes.clear();
			current_time = market_message.time();
		}
        
        if (message_type_data[market_message.type()].cur_time != current_time)
        {
            message_type_data[market_message.type()].time_count ++;
            message_type_data[market_message.type()].cur_time = current_time;
        }
    }
    

    for (map<uint32_t,messages_counts_data>::iterator it=message_type_data.begin(); it!=message_type_data.end(); ++it)
    {
        messages_counts_data data = it->second;
        
        const uint32_t message_count = data.messages_count;
        const uint32_t time_count = data.time_count;
        const uint32_t message_type = it->first;

        if (time_count > 0)
        {
            outputFile.write((char*)&message_type, sizeof( boost::uint32_t ) );
            
            double average_message_count = static_cast<double>( message_count ) / time_count;
            
            outputFile.write( (char*)&average_message_count, sizeof( average_message_count));
        }
      
    }
    
    inputFile.close();
    outputFile.close();
 }



int main(int argc, const char * argv[])
{

	//generate_input_files();

	for( size_t i = 1; i < 1000; i++ )
 	{	
 		boost::thread t(processing_file, i );
		t.join();
 	}
 
    return 0;
}

