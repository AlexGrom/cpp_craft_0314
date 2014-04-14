

#include <iostream>
#include <fstream>
#include <market_message.h>
#include <vector>
#include <algorithm>
#include <boost/strong_typedef.hpp>
#include <map>

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



int main(int argc, const char * argv[])
{
    ifstream inputFile(BINARY_DIR"/input.txt", ios::binary);
    
    if (!inputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		exit(1);
	}
    
    ofstream outputFile(BINARY_DIR"/output.txt", ios::binary);
    
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
  
    return 0;
}

