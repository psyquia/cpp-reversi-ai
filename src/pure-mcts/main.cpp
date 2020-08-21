#include <iostream>
#include <iomanip>      
#include <string>
#include <stdio.h>
#include <vector>
#include <set>
#include "definitions.h"
#include "board.h"
#include "myhelper.h"
#include "ai.h"

#define X 1
#define O 0

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------


typedef map< string, vector< vector<int> > > flip_map;

set<string> commands = {
	"exit"
};

// ---------------------------------------------------------------------------------------------------------------------


flip_map getCmdFromValidMoves(vector<flip_info> flip_arr){
	flip_map valid_moves = {};
	for(auto move : flip_arr){
		valid_moves.insert({indexToStr(move.pos[0],move.pos[1]), 
					move.to_flip});
	}
	return valid_moves;
}

// ---------------------------------------------------------------------------------------------------------------------


void benchmark(){
	const int testlen = 10;
	Board b;
	Ai ai(0);

	vector<flip_info> valid_moves = b.getMoves(0);

	for(int i=0; i<testlen; i++){
		ai.findBestMove(b, valid_moves);
	}
}

// ---------------------------------------------------------------------------------------------------------------------


void auto_move(Board &b, vector<flip_info> &vmoves, int team){
	cout << "\nCHOSE RANDOMLY\n";

	int aut = rand() % vmoves.size();

	b.doMoveIndex(vmoves[aut].pos, vmoves[aut].to_flip, team);
}

// ---------------------------------------------------------------------------------------------------------------------


void game_over(Board &b, bool &running, string &input, int player){
	b.printBoard({});
	cout << "\nGAME OVER\n\n";
	if(b.getWinner() == player){
		cout << "\u001b[34mYOU WIN\u001b[0m\n";
	}else{
		cout << "\u001b[31mYOU LOSE\u001b[0m\n";
	}

	cout << "\nPLAY AGAIN? (y/n): ";
	getline(cin,input);

	while(my_tolower(input) != "y" && my_tolower(input) != "n"){
		cout << "invalid input \n" << "PLAY AGAIN? (y/n): ";
		getline(cin,input);
	}

	if(my_tolower(input) == "n"){
		running = false;
	}
	cout << endl;
}

// ---------------------------------------------------------------------------------------------------------------------


void reversi(){
	// Random seed
	srand(time(NULL));

	bool running = true;
	string sig = ">> ";

	int player, turn = rand() % 2;
	string *inputp = new string();
	string &input = *inputp;


	while(running)
	{
		cout << "Would you like to be ○ (X) or ● (O) ?\n" << sig;
		getline(cin,input);

		while(my_tolower(input) != "x" && my_tolower(input) != "o"){
			if(input == "exit"){
				running = false;
				break;
			}
			cout << "Invalid input \n" << sig;
			getline(cin,input);
		}
		if(!running) break;

		player = (my_tolower(input) == "x") ? X : O;
		
		Board b;
		Ai ai(!player);

		while(true){
			vector<flip_info> valid_moves = b.getMoves(turn);

			if(valid_moves.size() == 0){
				game_over(b,running,input,player);
				break;
			}

			if(turn == player){
				auto valid_cmds = getCmdFromValidMoves(valid_moves);
				b.printBoard(valid_moves);
				if(input == "exit"){
					running = false;
					break;
				}
				cout << sig << "\u001b[34mYOUR TURN:\u001b[0m (choose a position *) ";
				getline(cin,input);

				#if AUTO_MOVE == true

				auto_move(b, valid_moves, turn);

				#endif

				#if AUTO_MOVE == false

				auto move = valid_cmds.find(input);
				while(move == valid_cmds.end()){
					if(input == "exit"){
						running = false;
						break;
					}
					cout << "Invalid position \n" << sig << "\u001b[34mYOUR TURN:\u001b[0m ";
					getline(cin,input);
					move = valid_cmds.find(input);
				}
				if(running == false){
					break;
				}

				cout <<"\n\n";

				b.doMove(move->first, move->second, turn);

				#endif	

			}else{

				b.printBoard({});

				auto ai_move = ai.findBestMove(b, valid_moves);

				b.doMoveIndex(ai_move.pos, ai_move.to_flip, turn);
			}

			turn = !turn;
		}
	}
	
	delete inputp;
}

// ---------------------------------------------------------------------------------------------------------------------


int main(int argc, char *argv[]){
	reversi();
}
