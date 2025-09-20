#ifndef Q2COROUTINE_H
#define Q2COROUTINE_H

#include <uPRNG.h>

class Potato {
	// YOU ADD MEMBERS HERE
	int timer;
	PRNG& prng;
	unsigned int maxTicks;
public:
	_Exception Explode {};
	Potato( PRNG & prng, unsigned int maxTicks = 10 );
	void reset( unsigned int maxTicks = 10 );
	void countdown();
};

_Coroutine Player {
	_Exception Terminate {
	public:
		Player & victim;					// delete player
		Terminate( Player & victim ) : victim( victim ) {}
	};
	_Exception Election {};

	Player * partner[2];					// left and right player
	// YOU ADD MEMBERS HERE
	Potato& potato;
	PRNG& sprng;
	unsigned int id;

	bool isUmpire();
	void vote();							// resume partner to vote
	void terminate();						// resume umpire
	void main();

public:
	static inline Player * umpire{nullptr}; // current umpire, define as nullptr to avoid global define

	Player( PRNG & prng, unsigned int id, Potato & potato );
	void init( Player & lp, Player & rp );	// supply partners
	unsigned int getId() const;					// player id
	void toss();							// tossed potato
};

#endif