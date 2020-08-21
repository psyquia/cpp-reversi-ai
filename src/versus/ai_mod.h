#pragma once

#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <math.h>

#include "board.h"
#include "state_mod.h"
#include "definitions.h"

/*! @brief Reversi AI Implementation using MCTS
 *
 * @details This class is used to find the next best move that the AI opponent can make
 *
 */
class Ai_mod{
public:
	int team;

	Ai_mod(int team);
	
	~Ai_mod();

	/*! @brief finds best move from possible moves
     *
     *  @param b 				game board
     *	@param possible_moves	vector of possible moves
     *	@return 				best move as a flip_info struct
     */
	flip_info findBestMove(Board b, std::vector<flip_info> possible_moves);
	
private:

	/*! @brief selects optimal node (selection phase)
     *
     *  @param root 			node for starting state
     *	@return 				optimal node
     */
	node_mod* chooseNode(node_mod *root);

	/*! @brief selects child with maximum UCT value
     *
     *  @param parent 			parent node
     *	@return 				child with best UCT value
     */
	node_mod* getBestChild(node_mod *parent);

	/*! @brief expands tree by adding child states of @param parent (expansion phase)
     *
     *  @param parent 			parent node
     */
	void expandNode(node_mod *parent);

	/*! @brief backpropagates up tree to update score values (update phase)
     *
     *  @param start 			starting node
     *  @param team 			winning team
     */
	void updateTree(node_mod *start, int team);

	/*! @brief performs a playout of game and returns winner
     *
     *  @param node 			node with starting state of playout
     *  @return 	 			team value of winner 
     */
	int randPlayout(node_mod *node);
};
