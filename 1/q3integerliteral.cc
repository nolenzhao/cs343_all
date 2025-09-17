#include <iostream> 
#include <cstring>
#include <fstream>
#include "q3helpers.h"
#include "q3coroutine.h"


int main(int argc, char* argv[]){

	std::istream* infile = &std::cin;		// Default input to stdin (command line)
	if(argc > 2){							// If there is more than just the infile, undefined 
		std::cout << "Usage: " << argv[0] << " [ INPUT_FILE ]" << std::endl;
		std::cout << "If no input file is specified, input comes from standard input" << std::endl;
		exit(EXIT_FAILURE);
	}
	if(argc == 2){							// User provides an infile
		try{
			infile = new std::ifstream();
			dynamic_cast<std::ifstream *>(infile)->open(argv[1]);	// Implementation quirk which requires this cast
		}
		catch( *infile.uFile::Failure &){							
			std::cerr << "File does not exist or cant be opened: " << argv[1] << std::endl;
			exit(EXIT_FAILURE);
		} 
	}

	std::string line;
	while(std::getline(*infile, line)){

		IntegerLiteral int_ltl;										// Initialize a new coroutine per line, passing a char one at a time into coroutine
		long unsigned int place = 0;								// Keep track of the place at which the coroutine parsed up to, to allow main to handle extraneous chars
		try{

			if(line.empty()){
				std::cout << "\"\" : Warning! Blank line.\n";
				continue;
			}
			_Enable{												// Enable non-local exceptions; placed inside try block to ensure exceptions can be caught 
				for(place = 0; place < line.size(); place++){		
					char c = line[place];
					int_ltl.next(c);							   // Pass characters one by one into coroutine
				}
				int_ltl.next(IntegerLiteral::EOT);				   // If the for loop exits, then no exception was thrown and we can pass EOT to force coroutine to throw a match
			}
		}
		catch(int_ltl.IntegerLiteral::Error& e){	// After the coroutine asserts a match or error, we can then check extra chars 

			if(place < line.size()){			// Account for not reaching EOT
				place += 1;
			}

			std::cout << "\"" << line << "\" : \""; 
			for(int i = 0; i < place; i++){
				std::cout << line[i];
			}
			std::cout << "\" no";
			printExtra(place, line);			// Print rest of characters that were not passed to routine;
		}
		catch(int_ltl.IntegerLiteral::Match& m){

			if(place < line.size()){			// Account for not reaching EOT
				place += 1;
			}

			std::cout << "\"" << line << "\" : \"";
			for(int i = 0; i < place; i++){
				std::cout << line[i];
			}
			std::cout << "\" yes, value " << m.value;
			printExtra(place, line);
		}
	}
	return EXIT_SUCCESS;	
}