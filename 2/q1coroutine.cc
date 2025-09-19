#include "q1coroutine.h"
#include "q1helper.h"
#include <fstream>
#include <iostream>

void Writer::main() {
    try{
        _Enable{
            for(;;){
                outfile << ch;
                total_chars++;
                suspend();
            }
        }
    }
    _CatchResume(Filter::Eof&){
        std::cout << total_chars << " characters\n";    
    }       // When this function exits, control returns to the starter which was the filter which called put() for this Writer (basically same as last resumer)
}           // The previous filter would have called the final put method, and then terminating, continuing the resumption to the it's starter all the way until the Reader 

void Reader::main(){
    std::string line;
    char ch;
    for(;;){
        infile >> ch;
        if(infile.eof()) break;

        next->put(ch);
        
    }
    _Resume Filter::Eof{} _At *next;
    next->put('a');     // Put a random char here, intent is to resume to the next filter all the way to the Writer, which will terminate and resume to it's starter back here
}

void Case::main(){

    try{
        _Enable{
            for(;;){
                switch(case_choice){
                    case CaseOption::LOWER:
                        next->put(tolower(ch));
                        break;
                    case CaseOption::UPPER:
                        next->put(toupper(ch));
                        break;
                    default:
                        throw std::logic_error("Unexpected Error\n");
                }
                suspend();
            }
        }
    }
    _CatchResume(Filter::Eof&){
        _Resume Filter::Eof{} _At *next;
        next->put('a');
    }
}


void Capitalize::main(){
    try{
        _Enable{
            for(;;){
                if(first_letter && isalpha(ch)){
                    first_letter = !first_letter;
                    next->put(toupper(ch));
                    suspend();
                }
                else if(isPunctuation(ch)){
                    next->put(ch);
                    suspend();

                    if(ch == ' '){
                        while(ch == ' '){
                            next->put(ch);
                            suspend();
                        }
                        next->put(toupper(ch));
                        suspend();
                    }
                    else{
                        next->put(ch);
                        suspend();
                    }
                } 
                else{
                    next->put(ch);
                    suspend();
                }
            }
        }
    }
    _CatchResume(Filter::Eof&){
        _Resume Filter::Eof{} _At *next;
        next->put('a');
    }
}

// TODO: wait for clarificatoin on this option
void Reverse::main(){

    try{
        _Enable{
            for(;;){
                while(ch == ' '){  // No transformation needed for whitespace, only reverse words
                    next->put(ch);
                    suspend();
                }
                if(isalpha(ch)){    // Start of a new word
                    next->put(ch);
                    suspend();
                }
                else{   // Not a word
                    
                }
                next->put(ch);
                suspend();
            }
        }
    }
    _CatchResume(Filter::Eof&){
        _Resume Filter::Eof{} _At *next;
        next->put('a');
    }
}

Writer::Writer(std::ostream& o) : Filter{nullptr}, outfile{o} {};
Case::Case(Filter* f, CaseOption case_choice) : Filter{f}, case_choice{case_choice} {};
Capitalize::Capitalize(Filter* f) : Filter{f}{};
Reverse::Reverse(Filter* f) : Filter{f}{};

/*
Start coroutine in constructor given Reader will be the last constructed and it can 
start immediately reading characters from input. This makes it so making a separate 
start function to initially resume the Reader unnecessary
*/
Reader::Reader(Filter* f, std::istream& i) : Filter{f}, infile{i} { resume(); };     