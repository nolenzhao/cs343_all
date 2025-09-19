#ifndef Q1COROUTINE_H
#define Q1COROUTINE_H

#include "q1helper.h"
#include <fstream> 



constexpr int MAX_WORD = 100;

_Coroutine Filter {
protected:
	_Exception Eof {};					// no more characters
	Filter * next;						// next filter in chain
	unsigned char ch;					// communication variable
public:
	Filter( Filter * next ) : next{ next } {}
	void put( unsigned char c ) {
		ch = c;
		resume();
	}
};

_Coroutine Reader : public Filter {
	// YOU ADD PRIVATE MEMBER
	std::istream& infile;

	void main() override final;

public:
	Reader( Filter * f, std::istream & i );
};

_Coroutine Writer : public Filter {
	// YOU ADD PRIVATE MEMBERS
    std::ostream& outfile;
	size_t total_chars = 0;
    void main() override final;
public:
	Writer( std::ostream & o );
};

_Coroutine Reverse : public Filter {
	// YOU ADD PRIVATE MEMBERS
	char word[MAX_WORD];
	bool validWord = true;

	void main() override final;
public:
	Reverse( Filter * f);
};

_Coroutine Capitalize : public Filter {
	// YOU ADD PRIVATE MEMBERS
	bool first_letter = true;
	void main() override final;
public:
	Capitalize( Filter * f);
};

_Coroutine Case : public Filter {
	// YOU ADD PRIVATE MEMBERS
	CaseOption case_choice;

	void main() override final;
public:
	Case( Filter * f, CaseOption case_choice);
};



#endif