#include "q2coroutine.h"
#include "q2helper.h"
#include <iostream> 
#include <unistd.h>

int main(int argc, char* argv[]){


	PRNG sprng;
	intmax_t seed = getpid();
	int games = DEFAULT_GAMES;
	int players = 0;

	try{
		switch(argc){
		case 4: 
			if(strcmp(argv[3], "d") != 0){
				intmax_t seed_arg = convert(argv[3]);
				if(seed_arg <= 0){
					throw seed_cmd_err{};
				}
				seed = seed_arg;
			}
			sprng.set_seed(seed);

		case 3: 
			if(strcmp(argv[2], "d") != 0){
				intmax_t players_arg = convert(argv[2]);
				if(players_arg <= 0 || players_arg > 50){
					throw player_cmd_err{};
				}
				players = players_arg
			}
			else{
				players = sprng(1, 50);
			}
		case 2: 
			if(strcmp(argv[1], "d") != 0){
				intmax_t games_arg = convert(argv[1]);
				if(games_arg < 0 || games_arg > 50){
					throw game_cmd_err{};
				}
			}
		case 1: 
			break;
		default:
			throw cmd_err{};
		}
	}
	catch(seed_cmd_err&){
		std::cerr << "Seed value must be greater than zero\n";
		exit(EXIT_FAILURE);
	}
	catch(player_cmd_err&){
		std::cerr << "Must have at least one player and 50 players or less\n";
		eixt(EXIT_FAILURE);
	}
	catch(game_cmd_err& ){
		std::cerr << "Game value must be between 0 and 50 inclusive\n";
		exit(EXIT_FAILURE);
	}
	catch(cmd_err&){
		std::cerr << "Unknown or insufficient command arguments\n";
		exit(EXIT_FAILURE);
	}



	return EXIT_SUCCESS;
}