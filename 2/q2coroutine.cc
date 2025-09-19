#include "q2coroutine.h"



Potato::Potato(PRNG& prng, unsigned int maxTicks = 10): prng{prng} { reset();}

void Potato::reset(unsigned int maxTicks = 10){
    timer = prng(1, maxTicks);
}