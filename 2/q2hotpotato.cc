#include "q2coroutine.h"
#include "q2helper.h"
#include <iostream> 
#include <unistd.h>


int main(int argc, char* argv[]){


	PRNG sprng1, sprng2, sprng3;
	intmax_t seed = getpid();
	int num_games = DEFAULT_GAMES;
	int num_players = 0;

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
			sprng1.set_seed(seed);
			sprng2.set_seed(seed);
			sprng3.set_seed(seed);

		case 3: 
			if(strcmp(argv[2], "d") != 0){
				intmax_t players_arg = convert(argv[2]);
				if(players_arg <= 0 || players_arg > 50){
					throw player_cmd_err{};
				}
				num_players = players_arg;
			}
			else{
				num_players = sprng1(1, 50);
			}
		case 2: 
			if(strcmp(argv[1], "d") != 0){
				intmax_t games_arg = convert(argv[1]);
				if(games_arg < 0 || games_arg > 50){
					throw game_cmd_err{};
				}
				num_games = games_arg;
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
		exit(EXIT_FAILURE);
	}
	catch(game_cmd_err& ){
		std::cerr << "Game value must be between 0 and 50 inclusive\n";
		exit(EXIT_FAILURE);
	}
	catch(cmd_err&){
		std::cerr << "Unknown or insufficient command arguments\n";
		exit(EXIT_FAILURE);
	}

	Potato potato{sprng2};

	std::vector<Player*>players;

	for(unsigned int id = 0; id < players.size(); id++){
		players.push_back(new Player{sprng3, id, potato});
	}

	/*
	Set the zero player to the umpire in the case that there is only one player, 
	but also to swap in the following when a random index is generated
	*/
	Player::umpire = players[0];

	/*
	Generate a random player index [1, players-1] swap, that with position 0
	Then do a scan through to check for id zero and set that to umpire;
	*/
	if(players.size() > 1){
		int swap_idx = sprng1(1, players.size() - 1);
		players[0] = players[swap_idx];
		players[swap_idx] = Player::umpire;		// Set zero id player as umpire
	}

	for(int i = 0; i < players.size(); i++){
		int prev = (i - 1 + players.size()) % players.size();
		int next = (i + 1) % players.size();
		players[i]->init(*players[prev], *players[next]);
	}

	Player::umpire->toss();								// Start the game 






	return EXIT_SUCCESS;
}