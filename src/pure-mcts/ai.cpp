#include <climits>
#include <omp.h>

#include "ai.h"

using namespace std;

// Count for playouts during an instance of Ai::findBestMove()
static int PLAYOUT_COUNT = 0;

// ---------------------------------------------------------------------------------------------------------------------


static float getScore(float w, float l, float d){
	return 1.01*w + d - l;
}

// ---------------------------------------------------------------------------------------------------------------------


Ai::Ai(int team): team(team) {}

// ---------------------------------------------------------------------------------------------------------------------


Ai::~Ai() {}

// ---------------------------------------------------------------------------------------------------------------------


node* Ai::chooseNode(node *root){
	node *cnode = root;
	while(cnode->children.size() > 0){
		cnode = getBestChild(cnode);
	}

	return cnode;
}

// ---------------------------------------------------------------------------------------------------------------------


node* Ai::getBestChild(node *parent){
	int p_visits = parent->visits;
	double nodeScore,best_nodeScore = 0;
	node * bestChild = parent->children[0];
	for(node *child : parent->children){
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


void Ai::expandNode(node *parent){
	vector<State> childStates = parent->state.getChildStates();
	for(auto state : childStates){
		node * child = new node(state, parent);
	}
}

// ---------------------------------------------------------------------------------------------------------------------


void Ai::updateTree(node *start, int team){
	node *curr = start;
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


int Ai::randPlayout(node *node){
	State simState = node->state;
	int winner = -1;

	if(simState.game_over){
		if(simState.getWinner() == !this->team)
			node->parent->state.score = -INT_MAX;
	}

	while(!simState.game_over){
		simState.doRandMove();
	}

	winner = simState.getWinner();

	#pragma omp atomic
	PLAYOUT_COUNT++;
	
	return winner;
}

// ---------------------------------------------------------------------------------------------------------------------


flip_info Ai::findBestMove(Board b, vector<flip_info> possible_moves){
	PLAYOUT_COUNT = 0;
	flip_info best_move;
	int best_score = -INT_MAX;
	int tick = 0;

	node *root = new node(State(b,!team));

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
		
		node *testNode;

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

		#pragma omp single nowait
		{
			tick++;
			if(tick % 50 == 0){
				cout << "=> \u001b[31mCOMPUTER TURN:\u001b[0m Simulated "
				 << PLAYOUT_COUNT << " games...\r";
				cout.flush();
			}
		}
	}

	cout << "=> \u001b[31mCOMPUTER TURN:\u001b[0m Simulated "
	 	 << PLAYOUT_COUNT << " games...\r";

	for(node *child : root->children){
		if(DEBUG)
			cout << " score:" << child->state.score << endl;
		if(child->state.score > best_score){
			best_move = child->state.move;
			best_score = child->state.score;
		}
	}

	delete root;

	cout << "\n\n"
		 << "\u001b[31mCOMPUTER CHOSE:\u001b[0m " << indexToStr(best_move.pos[0],best_move.pos[1]) << endl;
	return best_move;
}
