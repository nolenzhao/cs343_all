#include "q2coroutine.h"



Potato::Potato(PRNG& prng, unsigned int maxTicks): prng{prng}, maxTicks{maxTicks} { reset();}

void Potato::reset(unsigned int maxTicks){
    timer = prng(1, maxTicks);
}

void Potato::countdown(){
    timer--;                // Every call to countdown ticks down by one
    if(timer == 0){
        throw Explode{};    // throw an Explode exception which will be caught by the Player when it resumes
    }
}



Player::Player(PRNG& prng, unsigned int id, Potato& potato)
    : potato{potato}, sprng{prng}, id{id} {};


void Player::vote(){
    try{
        _Enable{

        }
    }
    _CatchResume(Election){
        if(getId())
        _Resume Election{} _At *partner[1];
        partner[1]->vote();
    }
}

void Player::terminate(){

    try{
        _Enable{

        }
    }
    _CatchResume(Terminate& t){
        if(t.victim.isUmpire()){
            _Resume Election{} _At *partner[1];
            partner[1]->vote();
        }
        // unlink player in the terminate ex
        // t.victim;          
        t.victim.partner[0]->partner[1] = t.victim.partner[1];
        t.victim.partner[1]->partner[0] = t.victim.partner[0];
        delete &t.victim;
        potato.reset(); 
        toss();
    }
}

void Player::main(){

    try{
        _Enable{
            while(true){
                suspend();
                potato.countdown();
                int dir = sprng(1);
                partner[dir]->toss();
            }
        }
    }
    _CatchResume(Potato::Explode){
        _Resume Terminate{*this} _At *umpire; 
        umpire->terminate();
    }
}

unsigned int Player::getId() const{
    return id;
}

void Player::toss(){
    resume();
}

void Player::init(Player& lp, Player& rp){
    partner[0] = &lp;
    partner[1] = &rp;
    resume();
}


bool Player::isUmpire(){
    return this == umpire;
}