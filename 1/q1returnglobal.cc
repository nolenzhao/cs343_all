#include <iostream>
#include <cstdint>										// intmax_t
#include <cstdlib>										// access: rand, srand
#include <cstring>										// access: strcmp
using namespace std;
#include <unistd.h>										// access: getpid

static intmax_t convert( const char * str ) {			// convert C string to integer
	char * endptr;
	errno = 0;											// reset
	intmax_t val = strtoll( str, &endptr, 10 );			// attempt conversion
	if ( errno == ERANGE ) throw std::out_of_range( "" );
	if ( endptr == str ||								// conversion failed, no characters generated
		 *endptr != '\0' ) throw std::invalid_argument( "" ); // not at end of str ?
	return val;
} // convert

struct Ex1 { short int code; };
struct Ex2 { int code; };
struct Ex3 { long int code; };

bool rtn3_flag = false;
bool rtn2_flag = false;
bool rtn1_flag = false;
long int ex3_v = 0;
int ex2_v = 0;
short int ex1_v = 0;

intmax_t eperiod = 10000;								// exception period
int randcnt = 0;
int Rand() { randcnt += 1; return rand(); }

double rtn1( double i ) {
	if ( Rand() % eperiod == 0 ) { 
		rtn1_flag = true;
		ex1_v = (short int)Rand();
		return 0;
	} // replace

	return i + Rand();
}
double rtn2( double i ) {
	if ( Rand() % eperiod == 0 ) { 
		rtn2_flag = true;
		ex2_v = Rand();
		return 0;
	} // replace

	double res = rtn1(i);

	if(rtn1_flag){
		return 0;
	}

	return res + Rand();
}
double rtn3( double i ) {
	if ( Rand() % eperiod == 0 ) {
		rtn3_flag = true;
		ex3_v = Rand();
		return 0;
	}	// replace

	double res = rtn2(i);

	if(rtn2_flag || rtn1_flag){
		return 0;
	}

	return res + Rand();
}

int main( int argc, char * argv[] ) {
	intmax_t times = 100000000, seed = getpid();		// default values
	struct cmd_error {};

	try {
		switch ( argc ) {
		  case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
			seed = convert( argv[3] ); if ( seed <= 0 ) throw cmd_error(); }
		  case 3: if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
			eperiod = convert( argv[2] ); if ( eperiod <= 0 ) throw cmd_error(); }
		  case 2: if ( strcmp( argv[1], "d" ) != 0 ) {	// default ?
			times = convert( argv[1] ); if ( times <= 0 ) throw cmd_error(); }
		  case 1: break;								// use all defaults
		  default: throw cmd_error();
		} // switch
	} catch( ... ) {
		cerr << "Usage: " << argv[0] << " [ times > 0 | d [ eperiod > 0 | d [ seed > 0 | d ] ] ]" << endl;
		exit( EXIT_FAILURE );
	} // try
	srand( seed );

	double rv = 0.0;
	int ev1 = 0, ev2 = 0, ev3 = 0;
	int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

	for ( int i = 0; i < times; i += 1 ) {
		double res = rtn3(i);

		if(!rtn1_flag && !rtn2_flag && !rtn3_flag){
			rv += res;
			rc += 1;
		}
		if(rtn1_flag){
			ev1 += ex1_v;
			ec1 += 1;
			rtn1_flag = false;
		}
		if(rtn2_flag){
			ev2 += ex2_v;
			ec2 += 1;
			rtn2_flag = false;
		}
		if(rtn3_flag){
			ev3 += ex3_v;
			ec3 += 1;
			rtn3_flag = false;
		}

	} // for
	cout << "randcnt " << randcnt << endl;
	cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2 << ' ' << ev3 << endl;
	cout << "calls "  << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3 << endl;
}
