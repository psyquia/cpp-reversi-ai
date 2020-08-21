#include "state_mod.h"

using namespace std;

/*! @brief positional value map
 */
int posue[8][8] = 
{
	{ T1,-T1, T3, 1, 1, T3,-T1, T1},
	{-T1,-T1, 1, 1, 1, 1,-T1,-T1},
	{ T3, 1, T2, 1, 1, T2, 1, T3},
	{ 1, 1, 1, 1, 1, 1, 1, 1},
	{ 1, 1, 1, 1, 1, 1, 1, 1},
	{ T3, 1, T2, 1, 1, T2, 1, T3},
	{-T1,-T1, 1, 1, 1, 1,-T1,-T1},
	{ T1,-T1, T3, 1, 1, T3,-T1, T1}
};


// ---------------------------------------------------------------------------------------------------------------------


State_mod::State_mod() {}

// ---------------------------------------------------------------------------------------------------------------------


State_mod::~State_mod() {}

// ---------------------------------------------------------------------------------------------------------------------


State_mod::State_mod(Board b, int team_p): board(b), team(team_p), score(0) {}

// ---------------------------------------------------------------------------------------------------------------------


State_mod::State_mod(Board b, flip_info mov, int team_p): board(b), move(mov), team(team_p), score(0) {}

// ---------------------------------------------------------------------------------------------------------------------


vector<State_mod> State_mod::getChildStates(){
	Board tmp_board;
	vector<State_mod> child_states = {};
	for(auto move : this->board.getMoves(!this->team)){
		tmp_board = board;
		tmp_board.doMoveIndex(move.pos, move.to_flip, !this->team);
		child_states.push_back(State_mod(tmp_board, move, !this->team));
	}
	return child_states;
}

// ---------------------------------------------------------------------------------------------------------------------


int State_mod::getWinner(){
	return this->board.getWinner();
}

// ---------------------------------------------------------------------------------------------------------------------


void State_mod::doMove(){
	int best_score = -INT_MAX;
	flip_info best_move;
	vector<flip_info> new_moves = this->board.getMoves(this->team);
	if(new_moves.size() == 0){
		this->game_over = true;
		return;
	}

	for(auto move : new_moves){
		int curr_score = posue[move.pos[1]][move.pos[0]];
		for(auto capture : move.to_flip){
			curr_score += posue[capture[1]][capture[0]];
		}
		if(curr_score > best_score){
			best_move = move;
			best_score = curr_score;
		}
	}

	// for(auto move : new_moves){
	// 	int curr_score = posue[move.pos[1]][move.pos[0]];
	// 	if(curr_score > best_score){
	// 		best_move = move;
	// 		best_score = curr_score;
	// 	}else if(curr_score == best_score){
	// 		best_move = (move.to_flip.size() > best_move.to_flip.size()) 
	// 					? move : best_move;
	// 	}
	// }

	// this->board.printBoard(new_moves);

	// cout << best_move.pos[0] << "," << best_move.pos[1] << endl;
	// cout << best_score << endl;
	this->board.doMoveIndex(best_move.pos, best_move.to_flip, this->team);

	this->team = !this->team;
}

// ---------------------------------------------------------------------------------------------------------------------


node_mod::node_mod(): children({}) {}

// ---------------------------------------------------------------------------------------------------------------------


node_mod::~node_mod() {
	for(auto node : this->children){
		delete node;
	}
}

// ---------------------------------------------------------------------------------------------------------------------


node_mod::node_mod(State_mod s): state(s), parent(NULL), children({}) {}

// ---------------------------------------------------------------------------------------------------------------------


node_mod::node_mod(State_mod s, node_mod *p): state(s), parent(p), children({}) {
	p->children.push_back(this);
}
