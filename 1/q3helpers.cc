#include "q3helpers.h"
#include <iostream>

/*
Validate chars before calling this function. 
Using ASCII table codes to convert each character to its integer representation
*/
int digitConv(char ch){
    if('0' <= ch && ch <= '9'){
        return ch - '0';
    }
    else if('a' <= ch && ch <= 'f'){
        return ch - 'a' + 10;
    }
    else{
        return ch - 'A' + 10;
    }
}


/*
Convert from some base to decimal by converting each place of the value string 
and summing for all indices of the value string, multiplying by the power of the base for each index
*/
uintmax_t convertToDec(std::string val_str, int base){
    size_t len = val_str.size();
    uintmax_t multiplier = 1;
    uintmax_t total_val = 0;
    for(int i = len - 1; i >= 0; i--){
        total_val += digitConv(val_str[i]) * multiplier;
        multiplier *= base;
    }
    return total_val;
}


bool isUnsignedSuffix(char ch){
    return ch == 'u' || ch == 'U';
}

bool isLongSuffix(char ch){
    return ch == 'l' || ch == 'L';
}

bool isValidDec(char ch){
    return isdigit(ch);
}

bool isValidHex(char ch){
    return isxdigit(ch);
}

bool isValidBin(char ch){
    return ch == '0' || ch == '1';
}

bool isValidOct(char ch){
    return isdigit(ch) && digitConv(ch) < 8;
}
void printExtra(long unsigned int place, std::string line){

    /*
    If the place index of the char is not equal to the line length, then there are unhandled characters which haven't been processed 
    before the coroutine exited. This means EOT was not hit, and the extraneous characters need to be printed.
    */
    if(place != line.size()){                        
        std::cout << " -- extraneous characters \"";
    }
    for(long unsigned int i = place; i < line.size(); i++){     // Print up until the end of line, if the entire line was processed before, this doesnt run
        std::cout << line[i];	
        if(i == line.size()-1){
            std::cout << "\"";
        }
    }	
    std::cout << "\n";
}
