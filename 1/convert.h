#ifndef CONVERT_H
#define CONVERT_H

static intmax_t convert( const char * str ) {			// convert C string to integer
	char * endptr;
	errno = 0;											// reset
	intmax_t val = strtoll( str, &endptr, 10 );			// attempt conversion
	if ( errno == ERANGE ) throw std::out_of_range( "" );
	if ( endptr == str ||								// conversion failed, no characters generated
		 *endptr != '\0' ) throw std::invalid_argument( "" ); // not at end of str ?
	return val;
} // convert


#endif