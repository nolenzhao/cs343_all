#ifndef Q3_HELPERS
#define Q3_HELPERS
#include <string>

int digitConv(char ch);

uintmax_t convertToDec(std::string val_str, int base);

bool isUnsignedSuffix(char ch);

bool isLongSuffix(char ch);

void printExtra(long unsigned int place, std::string line);

bool isValidDec(char ch);

bool isValidHex(char ch);

bool isValidBin(char ch);

bool isValidOct(char ch);
#endif