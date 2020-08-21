#include "state.h"

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------


State::State() {}

// ---------------------------------------------------------------------------------------------------------------------


State::~State() {}

// ---------------------------------------------------------------------------------------------------------------------


State::State(Board b, int team_p): board(b), team(team_p), score(0) {}

// ---------------------------------------------------------------------------------------------------------------------


State::State(Board b, flip_info mov, int team_p): board(b), move(mov), team(team_p), score(0) {}

// ---------------------------------------------------------------------------------------------------------------------


vector<State> State::getChildStates(){
	Board tmp_board;
	vector<State> child_states = {};
	for(auto move : this->board.getMoves(!this->team)){
		tmp_board = board;
		tmp_board.doMoveIndex(move.pos, move.to_flip, !this->team);
		child_states.push_back(State(tmp_board, move, !this->team));
	}
	return child_states;
}

// ---------------------------------------------------------------------------------------------------------------------


int State::getWinner(){
	return this->board.getWinner();
}

// ---------------------------------------------------------------------------------------------------------------------


void State::doRandMove(){
	vector<flip_info> new_moves = this->board.getMoves(this->team);
	if(new_moves.size() == 0){
		this->game_over = true;
		return;
	}

	int choice = rand() % new_moves.size();

	this->board.doMoveIndex(new_moves[choice].pos, new_moves[choice].to_flip, this->team);

	this->team = !this->team;
}

// ---------------------------------------------------------------------------------------------------------------------


node::node(): children({}) {}

// ---------------------------------------------------------------------------------------------------------------------


node::~node() {
	for(auto node : this->children){
		delete node;
	}
}

// ---------------------------------------------------------------------------------------------------------------------


node::node(State s): state(s), parent(NULL), children({}) {}

// ---------------------------------------------------------------------------------------------------------------------


node::node(State s, node *p): state(s), parent(p), children({}) {
	p->children.push_back(this);
}