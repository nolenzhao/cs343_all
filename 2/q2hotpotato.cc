class Potato {
	// YOU ADD MEMBERS HERE
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
	void vote();							// resume partner to vote
	void terminate();						// resume umpire
	void main();
  public:
	static Player * umpire;					// current umpire

	Player( PRNG & prng, unsigned int id, Potato & potato );
	void init( Player & lp, Player & rp );	// supply partners
	unsigned int getId();					// player id
	void toss();							// tossed potato
};
