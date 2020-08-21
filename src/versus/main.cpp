#include <iostream>
#include <iomanip>      
#include <string>
#include <stdio.h>
#include <vector>
#include <set>

#include "definitions.h"
#include "board.h"
#include "myhelper.h"
#include "ai_mod.h"
#include "ai.h"


using namespace std;


static int pure_wins=0, mod_wins=0;


void game_over(Board &b, bool &running, string &input, int player);


int main(int argc, char *argv[]){
	// Random seed
	srand(time(NULL));

	bool running = true;
	string sig = ">> ";
	string input = "";

	int p_team = 0, turn = rand() % 2;



	for(int k=0; k<NUM_OF_GAMES; k++)
	{
		Board b;

		Ai ai_pure(p_team);
		Ai_mod ai_mod(!p_team);

		while(true){
			vector<flip_info> valid_moves = b.getMoves(turn);

			if(valid_moves.size() == 0){
				game_over(b,running,input,p_team);
				break;
			}

			if(turn == p_team){
				if(DEBUG)
					b.printBoard(valid_moves);

				auto ai_move = ai_pure.findBestMove(b, valid_moves);

				b.doMoveIndex(ai_move.pos, ai_move.to_flip, turn);

			}else{

				if(DEBUG)
					b.printBoard(valid_moves);

				auto ai_move = ai_mod.findBestMove(b, valid_moves);

				b.doMoveIndex(ai_move.pos, ai_move.to_flip, turn);
			}

			turn = !turn;
		}

		cout << "PURE MCTS WINS: " << pure_wins << endl;
		cout << "MODDED MCTS WINS: " << mod_wins << endl;
	}
}

void game_over(Board &b, bool &running, string &input, int pure){
	b.printBoard({});
	cout << "\nGAME OVER\n\n";
	if(b.getWinner() == pure){
		pure_wins++;
		cout << "\u001b[34mPURE MCTS WINS!\u001b[0m\n";
	}else{
		mod_wins++;
		cout << "\u001b[31mMODDED MCTS WINS!\u001b[0m\n";
	}
	cout << endl;
}
