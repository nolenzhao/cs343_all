#include "q1coroutine.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>



int main(int argc, char* argv[]){

	std::istream* infile = &std::cin;	// Default input is stdin
	std::ostream* outfile = &std::cout;	// Default output is stdout
	std::vector<char> options;			// Keep track of order of options while parsing	

	struct cmd_err{};
	
	size_t argPtr;
	try{
		for(argPtr = 1; argPtr < argc; argPtr++){      // Set argPtr = 1 to skip executable name
			if(argv[argPtr][0] != '-' || strlen(argv[argPtr]) > 2){      // Not an option if it doesnt start with "-" or longer than 2 chars
				break;
			}

			switch(argv[argPtr][1]){
			case 'c': 
				/*
				Assume whitespace separates "-c" and "u" or "l"
				Check first no out of bounds; short circuit so no indexing error will occur
				Check an "l" or "u" is attached, and nothing 
				*/
				argPtr += 1;		// Advance to the next char, grabbing presumably "l" or "u"
				if(argPtr >= argc || !(strcmp(argv[argPtr], "u") == 0 || strcmp(argv[argPtr], "l") == 0)){
					throw cmd_err{};
				}
				options.push_back(argv[argPtr][0]);		// Use "u" or "l" as an indicator of the specific filter type of the case filter
				break;
			case 'R': 
				options.push_back('R');
				break;
			case 's': 
				options.push_back('s');
				break;
			default: 
				throw cmd_err{};
			}
		}

		if(argPtr < argc){			// If there are arguments left, first must be input file
			infile = new std::ifstream();	
			dynamic_cast<std::ifstream*>(infile)->open(argv[argPtr]);
			argPtr += 1;
		}

		if(argPtr < argc){			// If there are still arguments left, must be the output file
			outfile = new std::ofstream();
			dynamic_cast<std::ofstream*>(outfile)->open(argv[argPtr]);
			argPtr += 1;
		}
		if(argPtr != argc){			// Any extra arguments is defined to be incorrect use of executable
			throw cmd_err{};
		}

	}
	catch(cmd_err& ){
		std::cerr << "Unknown or insufficient command arguments\n";
		std::cerr << "Usage: " << argv[0] << "[ - filter-options...] [ infile [outfile]]\n";
		exit(EXIT_FAILURE);
	}
	catch(*infile.uFile::Failure& ){
		std::cerr << "File does not exist or cannot be opened: " << argv[argPtr] << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(*outfile.uFile::Failure&){
		std::cerr << "File does not exist or cannot be opened: " << argv[argPtr] << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(...){
		std::cerr << "Unexpected error!\n";
		std::cerr << "Usage: " << argv[0] << "[ - filter-options...] [ infile [outfile]]\n";
		exit(EXIT_FAILURE);
	}

	/*
	Build the pipline from writer to read, in reverse order to the data flow. 
	This is necessary given each filter is passed to the constructor of its predecessor in 
	the pipeline. The writer does not require a filter, only ofstream in its' constructor
	*/
	std::vector<Filter*> pipeline;			

	pipeline.push_back(new Writer{*outfile});

	for(int i = options.size() - 1; i >= 0; i--){
		switch(options[i]){
		case 'u': 				// Denoted case -c u as 'u' char above
			pipeline.push_back(new Case{pipeline.back(), CaseOption::UPPER});
			break;
		case 'l': 				// Denoted case -c l as 'l' char above
			pipeline.push_back(new Case{pipeline.back(), CaseOption::LOWER});
			break;
		case 'R': 
			pipeline.push_back(new Reverse{pipeline.back()});
			break;
		case 's': 
			pipeline.push_back(new Capitalize{pipeline.back()});
			break;
		}
	}

	*infile >> std::noskipws;
	pipeline.push_back(new Reader{pipeline.back(), *infile});	// Reader's constructor calls resume()


	return EXIT_SUCCESS;
}