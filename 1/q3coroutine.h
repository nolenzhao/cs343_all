#ifndef Q3_COROUTINE
#define Q3_COROUTINE

// First we check if we are making dec, bin, oct, hex literal since the suffix is always added to the end
inline constexpr int MAX_BIN = 64;
inline constexpr int MAX_HEX = 16;
inline constexpr int MAX_DEC = 20;
inline constexpr int MAX_OCT = 23;

_Coroutine IntegerLiteral {
	char ch;								// character passed by cocaller
	std::string val_str;
	int base = 10;							// Default base 10

	void handleTerminator();
	void throwError();

    void main();
	// YOU ADD MEMBERS HERE
public:
	enum { EOT = '\003' };				// end of text
	_Exception Match {					// last character match
	  public:
		uintmax_t value;				// value of integer literal
		Match( uintmax_t value ) : value( value ) {}
	};
	_Exception Error {};				// last character invalid
	void next( char c ) {
		ch = c;							// communication input
		resume();						// activate
	}
};

#endif