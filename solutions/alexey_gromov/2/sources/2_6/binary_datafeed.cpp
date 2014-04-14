// Laba_2_6.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <boost/cstdint.hpp>
#include "stock_data.h"
#include "market_message.h"

using namespace std;


class out_stock_data
{
	char stock_name_[ 9 ];
	boost::uint_fast32_t date_time_;
	double price_;
	boost::uint32_t volume_;
	double f2_;

	boost::uint32_t date_to_uint(const char date_time[8])
	{
		unsigned int year, month, day;

		sscanf( date_time, "%4d%2d%2d", &year, &month, &day );

		const boost::uint32_t ret_date_time = ( year - 1 ) * 372 + ( month - 1 ) * 31 + day;

		return ret_date_time;
	}

	void set_stock_name( const char *new_stock_name )
	{
		memset( stock_name_, 0, sizeof( stock_name_ ) );
		size_t len = strlen( new_stock_name );
		if( len > sizeof( stock_name_ ) )
		{
			len = sizeof(stock_name_) -1;
		}

		strncpy( stock_name_, new_stock_name, len );
	};

public:
	out_stock_data( binary_reader::stock_data& data )
	{
		set_stock_name( data.stock_name() );
		date_time_ = date_to_uint( data.date_time() );
		price_ = data.vwap();
		volume_ = data.volume();
		f2_ = data.f2();
	};

	void write( ofstream& out ) const
	{
		out.write( stock_name_, sizeof( stock_name_ ) );
		out.write( reinterpret_cast<const char*>( &date_time_ ), sizeof( date_time_ ) );
		out.write( reinterpret_cast<const char*>( &price_ ), sizeof( price_ ) );
		out.write( reinterpret_cast<const char*>( &volume_ ), sizeof( volume_ ) );
		out.write( reinterpret_cast<const char*>( &f2_ ), sizeof( f2_ ) );
	}
};

int main(int argc, char* argv[])
{
	
	ifstream inputFile(BINARY_DIR"/input.txt", ios::binary);
	if (!inputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		exit(1);
	}
    ofstream outputFile(BINARY_DIR"/output.txt", ios::binary);
	if (!outputFile)
	{
		cerr<<"File can not be opened \n"<<endl;
		exit(1);
	}
	
    while (!inputFile.eof())
    {
		binary_reader::stock_data data(inputFile);

		if( inputFile.eof() )
		{
			break;
		}

	    out_stock_data data_out(data);
		data_out.write(outputFile);
    }


	inputFile.close();
	outputFile.close();

	return 0;
}

