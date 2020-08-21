#include <climits>
#include <omp.h>

#include "ai_mod.h"

using namespace std;

// Count for playouts during an instance of Ai_mod::findBestMove()
static int PLAYOUT_COUNT = 0;

// ---------------------------------------------------------------------------------------------------------------------


static float getScore(float w, float l, float d){
	return 1.01*w + d - l;
}

// ---------------------------------------------------------------------------------------------------------------------


Ai_mod::Ai_mod(int team): team(team) {}

// ---------------------------------------------------------------------------------------------------------------------


Ai_mod::~Ai_mod() {}

// ---------------------------------------------------------------------------------------------------------------------


node_mod* Ai_mod::chooseNode(node_mod *root){
	node_mod *cnode = root;
	while(cnode->children.size() > 0){
		cnode = getBestChild(cnode);
	}

	return cnode;
}

// ---------------------------------------------------------------------------------------------------------------------


node_mod* Ai_mod::getBestChild(node_mod *parent){
	int p_visits = parent->visits;
	double nodeScore,best_nodeScore = 0;
	node_mod * bestChild = parent->children[0];
	for(node_mod *child : parent->children){
		if(child->visits == 0){
			return child;
		}
		nodeScore = ((double)child->state.score/(double)child->visits) 
					+ 1.41 * sqrt(log(p_visits)/(double)child->visits);
		if(nodeScore > best_nodeScore){
			bestChild = child;
			best_nodeScore = nodeScore;
		}
	}
	return bestChild;
}

// ---------------------------------------------------------------------------------------------------------------------


void Ai_mod::expandNode(node_mod *parent){
	vector<State_mod> childStates = parent->state.getChildStates();
	for(auto state : childStates){
		node_mod * child = new node_mod(state, parent);
	}
}

// ---------------------------------------------------------------------------------------------------------------------


void Ai_mod::updateTree(node_mod *start, int team){
	node_mod *curr = start;
	while(curr){
		curr->visits++;
		if(curr->state.team == team){
			#pragma omp atomic
			curr->state.score++;
		}
		curr = curr->parent;
	}
}

// ---------------------------------------------------------------------------------------------------------------------


int Ai_mod::randPlayout(node_mod *node){
	State_mod simState = node->state;
	int winner = -1;

	if(simState.game_over){
		if(simState.getWinner() == !this->team)
			node->parent->state.score = -INT_MAX;
	}

	while(!simState.game_over){
		simState.doMove();
	}

	winner = simState.getWinner();

	#pragma omp atomic
	PLAYOUT_COUNT++;
	
	return winner;
}

// ---------------------------------------------------------------------------------------------------------------------


flip_info Ai_mod::findBestMove(Board b, vector<flip_info> possible_moves){
	PLAYOUT_COUNT = 0;
	flip_info best_move;
	int best_score = -INT_MAX;
	int tick = 0;

	node_mod *root = new node_mod(State_mod(b,!team));

	auto now = chrono::system_clock::now();
	auto timeout = now + chrono::seconds(AI_TURN_LEN);


	#pragma omp parallel
	while(1){
		chrono::time_point<chrono::system_clock> l_now;

		#pragma omp critical
		l_now = now;
		if(l_now > timeout){
			break;
		}
		if(PLAYOUT_COUNT > 30000){
			break;
		}

		node_mod *testNode;

		#pragma omp critical
		{
			testNode = chooseNode(root);

			if(!testNode->state.game_over){
				expandNode(testNode);
				int num_children = testNode->children.size();
				if(num_children > 0){
					testNode = testNode->children[rand() % num_children];
				}
			}
		}


		now = chrono::system_clock::now();

		int winner = randPlayout(testNode);

		updateTree(testNode, winner);
		
		#if DEBUG

		#pragma omp single nowait
		{
			tick++;
			if(tick % 50 == 0){
				cout << "=> \u001b[31mMODDED MCTS TURN:\u001b[0m Simulated "
				 << PLAYOUT_COUNT << " games...\r";
				cout.flush();
			}
		}

		#endif
	}

	#if DEBUG

	cout << "=> \u001b[31mMODDED MCTS TURN:\u001b[0m Simulated "
	 	 << PLAYOUT_COUNT << " games...\r";
	
	#endif

	
	for(node_mod *child : root->children){
		if(child->state.score > best_score){
			best_move = child->state.move;
			best_score = child->state.score;
		}
	}

	delete root;

	#if DEBUG
	cout << "\n\n"
		 << "\u001b[31mCOMPUTER CHOSE:\u001b[0m " << indexToStr(best_move.pos[0],best_move.pos[1]) << endl;
	#endif
		 
	return best_move;
}
