#ifndef Q1HELPER_H
#define Q1HELPER_H

bool isPunctuation(char ch);

struct cmd_err{};

intmax_t convert( const char * str );			// convert C string to integer

enum class CaseOption{
	LOWER, 
	UPPER
};


#endif