#include <iostream>
#include <cstdint>										// intmax_t, uintmax_t
using namespace std;

static volatile uintmax_t shared = 0;					// volatile to prevent dead-code removal
static uintmax_t iterations = 1'000'000'000;

_Task increment {
	void main() {
		for ( decltype(iterations) i = 0; i < iterations; i += 1 ) {
			shared += 1;
		} // for
	} // increment::main
}; // increment

int main( int argc, char * argv[] ) {
	intmax_t processors = 1;
	struct cmd_error {};
	try {												// process command-line arguments
		switch ( argc ) {
		  case 3: processors = convert( argv[2] ); if ( processors <= 0 ) throw cmd_error{};
		  case 2: iterations = convert( argv[1] ); if ( iterations <= 0 ) throw cmd_error{};
		  case 1: break;								// use defaults
		  default: throw cmd_error{};
		} // switch
	} catch( ... ) {
		cerr << "Usage: " << argv[0] << " [ iterations (> 0) [ processors (> 0) ] ]" << endl;
		exit( EXIT_FAILURE );							// TERMINATE!
	} // try

	uProcessor p[processors - 1];						// create additional kernel threads
	{
		increment t[processors == 1 ? 2 : processors];
	} // wait for tasks to finish
	cout << "shared:" << shared << endl;
} // main
