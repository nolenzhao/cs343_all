#include "q3coroutine.h"
#include "q3helpers.h"
void IntegerLiteral::main(){

    if(!isdigit(ch)){ 					// First char needs to be a digit {0..9}
        throwError();
    }

    if(digitConv(ch)){                  // Any non-zero value will means this is a decimal-literal 
        base = 10;
        for(;;){
            if(ch == '\''){             // This will not be true on the first entrance. Only on subsequence chars
                suspend();              // If we meet single quote, make sure next one is a digit, simply suspend and request next char 

                if(!isValidDec(ch)){       // An unsandwiched quote is always an error
                    throwError();
                }
            }
            if(!isValidDec(ch)){           // If not a valid digit, or quote as handled above, it's a terminator
                handleTerminator();
            }
            val_str += ch;
            if(val_str.size() > MAX_DEC){
                throwError();
            }
            suspend();                  // We are done with char, so we suspend to get the next char
        }
    }
    else{ 								//  Zero value means this is oct, bin, or hex-literal					
        suspend();
        if(ch == 'b' || ch == 'B'){ 		// Binary Literal
            base = 2;
            suspend();                      // Ensure there is at least one binary digit (can't just be 0b)
            if(!isValidBin(ch)){
                throwError();
            }
            else{
                val_str += ch;
            }
            for(;;){
                suspend();
                if(val_str.size() > MAX_BIN){
                    throwError();
                }
                if(ch == '\''){             
                    suspend();                           // If we meet single quote, make sure next one is a digit, simply suspend and request next char 

                    if(!isValidBin(ch)){       // An unsandwiched quote is always an error
                        throwError();
                    }
                }
                if(!isValidBin(ch)){           // If ch not a quote as handled above, must be a terminator 
                    handleTerminator();
                }
                val_str += ch;
            }
        }
        else if(ch == 'x' || ch == 'X'){	// Hexadecimal Literal
            base = 16;
            suspend();                      // Make sure theres at least one hex digit after 0x
            if(!isValidHex(ch)){
                throwError();
            }
            else{
                val_str += ch;
            }
            for(;;){
                if(val_str.size() > MAX_HEX){
                    throwError();
                }
                suspend();
                if(ch == '\''){             
                    suspend();               // If we meet single quote, make sure next one is a digit, simply suspend and request next char 
                    if(!isValidHex(ch)){       // An unsandwiched quote is always an error
                        throwError();
                    }
                }
                if(!isValidHex(ch)){          // If the char is not a quote as handled above, it must be a terminator like OET or int suffix
                    handleTerminator();
                }
                val_str += ch;
            }
        }
        else{								// Octal Literal
            base = 8;
            val_str += ch;                     // Add 0 to the val_str for octal since the zero counts as a digit in octal 
            for(;;){
                if(ch == '\''){             
                    suspend();                 // If we meet single quote, make sure next one is a digit, simply suspend and request next char 
                    if(!isValidOct(ch)){       // An unsandwiched quote is always an error
                        throwError();
                    }
                }
                /*
                If the character is not a valid digit, or single quote as handled above, it must be a terminator like EOT or suffix
                Otherwise, it gets added to the value string
                */
                if(!isValidOct(ch)){           
                    handleTerminator();
                }
                val_str += ch;
                if(val_str.size() > MAX_OCT){                    // Check that the value string doesn't have too many digits
                    throwError();
                }
                suspend();
            }
        }
    }
}						// coroutine main


void IntegerLiteral::handleTerminator(){

    if(ch == EOT){                      
        _Resume Match{convertToDec(val_str, base)} _At resumer();       // Return a match by converting the important digits to decimal and throwing 
        suspend();
    }
    else if(isUnsignedSuffix(ch)){                                      // Path for if "u" comes first as a terminator
        suspend();
        if(ch == EOT){                                                  // OK to have just "u" as suffix
            _Resume Match{convertToDec(val_str, base)} _At resumer();
            suspend();
        }
        else if(isLongSuffix(ch)){                                      // If "l" is the next suffix, immediately match since there are no more possible digits
            _Resume Match{convertToDec(val_str, base)} _At resumer();
            suspend();
        }
        else{
            throwError();                                               // Throw because no other possible character could match or continue this sequence 
        }
    }
    else if(isLongSuffix(ch)){                                          // Path if "l" comes first
        suspend();
        if(ch == EOT){                                                  // OK to have just "l" as suffix
            _Resume Match{convertToDec(val_str, base)} _At resumer();
            suspend();
        }
        else if(isUnsignedSuffix(ch)){                                  //   Immediately match since there are no more possible digits
            _Resume Match{convertToDec(val_str, base)} _At resumer();   
            suspend();
        }
        else{
            throwError();                                               //  Throw Error as no other character could continue this sequence in a valid way
        }
    }
    else{
        throwError();                                                   //  Throw since only EOT, u, l could be posisble terminators
    }
}


void IntegerLiteral::throwError(){
    _Resume Error{} _At resumer();
    suspend();
}