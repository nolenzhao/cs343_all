#include <stdio.h>
#include <unistd.h>										// access: getpid
#include <errno.h>                                      // allow for use of errno
#include <stdint.h>										// intmax_t
#include <stdlib.h>										// access: rand, srand
#include <string.h>										// access: strcmp



typedef struct { char* code; } ExRange;
typedef struct { char* code; } ExEOS;


union ConvertRes {
    int i;
    ExRange ex_range;
    ExEOS ex_eos;
};

enum ConvertType{
    INT, 
    EX_RANGE, 
    EX_EOS
};

typedef struct {
    enum ConvertType type;
    union ConvertRes res;
} TaggedConvert;

typedef struct { short int code; } Ex1;
typedef struct { int code; } Ex2;
typedef struct { long int code; } Ex3;

union Q1Res {
    double val;
    Ex1 ex1;
    Ex2 ex2;
    Ex3 ex3;
};

enum Q1Type {
    DOUBLE, 
    EX1, 
    EX2, 
    EX3
};

typedef struct {
    enum Q1Type type;
    union Q1Res res;
} TaggedReturn;


static TaggedConvert convert( const char * str ) {			// convert C string to integer
	char * endptr;
	errno = 0;											// reset
	long long int val = strtoll( str, &endptr, 10 );			// attempt conversion
	if ( errno == ERANGE ) {
        TaggedConvert ret = {.type = EX_RANGE, .res = {.ex_range = {.code = ""}}};
        return ret;
    }
	if ( endptr == str || *endptr != '\0'){								// conversion failed, no characters generated
        TaggedConvert ret = {.type = EX_EOS, .res = {.ex_eos = {.code = ""}}}; // not at end of str ?
        return ret;
    }
    TaggedConvert ret = {.type = INT, .res = {.i = val}};
	return ret;
} // convert



long int eperiod = 10000;								// exception period
int randcnt = 0;
int Rand() { randcnt += 1; return rand(); }

TaggedReturn rtn1( double i ) {

    if ( Rand() % eperiod == 0) {
        TaggedReturn ex = {.type = EX1, .res = {.ex1 = Rand()}};
        return ex;
    }

    TaggedReturn ret = {.type = DOUBLE, .res = {.val = i + Rand()}};
    return ret;
}

TaggedReturn rtn2( double i ) {
    if ( Rand() % eperiod == 0 ) {
        TaggedReturn ex = {.type = EX2, .res = {.ex2  = Rand()}};
        return ex;
    } // replace

    TaggedReturn ret = rtn1(i);

    if(ret.type == DOUBLE){
        ret.res.val += Rand();
    }

	return ret;
}
TaggedReturn rtn3( double i ) {
    if ( Rand() % eperiod == 0 ) {
        TaggedReturn ex = { .type =EX3, .res = {.ex3 = Rand()}};
        return ex;
    }	// replace

    TaggedReturn ret = rtn2(i);

    if(ret.type == DOUBLE){
        ret.res.val += Rand();
    }

    return ret;
}

static TaggedConvert convert( const char * str );			// copy from https://student.cs.uwaterloo.ca/~cs343/examples/convert.h

int main( int argc, char * argv[] ) {
	long int times = 100000000, seed = getpid();		// default values
	struct cmd_error {};

    switch(argc){
        case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
            TaggedConvert seed_arg = convert(argv[3]);

            if(seed_arg.type == EX_RANGE || seed_arg.type == EX_EOS){
                goto BK_CMD_ERR; // caught at cmd_err
            }
            if (seed_arg.res.i <= 0 ){
               goto BK_CMD_ERR;  // throw cmd_err
            } 
            seed = seed_arg.res.i;
        }
        case 3: if ( strcmp( argv[2], "d") != 0 ){
            TaggedConvert eperiod_arg = convert(argv[2]);

            if (eperiod_arg.type == EX_RANGE || eperiod_arg.type == EX_EOS){
                goto BK_CMD_ERR;
            }
            if(eperiod_arg.res.i <= 0){
                goto BK_CMD_ERR;
            }
        } 
        case 2: if(strcmp( argv[1], "d") != 0){
            TaggedConvert times_arg = convert(argv[1]);

            if(times_arg.type == EX_RANGE || times_arg.type == EX_EOS){
                goto BK_CMD_ERR;
            }
            if(times_arg.res.i <= 0){
                goto BK_CMD_ERR;
            }
        }
        case 1: goto HAPPY_PATH;
        default: goto BK_CMD_ERR;
    } 
    BK_CMD_ERR:
    printf("Usage: %s [ times > 0 | d [ eperiod > 0 | d [ seed > 0 | d ] ] ]\n", argv[0]);
    exit( EXIT_FAILURE );


    HAPPY_PATH:
	srand( seed );

	double rv = 0.0;
	int ev1 = 0, ev2 = 0, ev3 = 0;
	int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

	for ( int i = 0; i < times; i += 1 ) {
        TaggedReturn ret = rtn3(i);

        if(ret.type == DOUBLE){
            rv += ret.res.val;
            rc += 1;
        }
        else if(ret.type == EX1){
            ev1 += ret.res.ex1.code;
            ec1 += 1;
        }
        else if(ret.type == EX2){
            ev2 += ret.res.ex2.code;
            ec2 += 1;
        }
        else{
            ev3 += ret.res.ex3.code;
            ec3 += 1;
        }
	} // for
	printf("randcnt %d\n", randcnt);
    printf("normal result %g exception results %d %d %d\n", rv, ev1, ev2, ev3);
    printf("calls %d exceptions %d %d %d\n", rc, ec1, ec2, ec3);

}
