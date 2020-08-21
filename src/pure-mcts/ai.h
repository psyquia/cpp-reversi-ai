#pragma once

#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <math.h>

#include "board.h"
#include "state.h"
#include "definitions.h"

/*! @brief Reversi AI Implementation using MCTS
 *
 * @details This class is used to find the next best move that the AI opponent can make
 *
 */
class Ai{
public:
	int team;

	Ai(int team);
	
	~Ai();

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
	node* chooseNode(node *root);

	/*! @brief selects child with maximum UCT value
     *
     *  @param parent 			parent node
     *	@return 				child with best UCT value
     */
	node* getBestChild(node *parent);

	/*! @brief expands tree by adding child states of @param parent (expansion phase)
     *
     *  @param parent 			parent node
     */
	void expandNode(node *parent);

	/*! @brief backpropagates up tree to update score values (update phase)
     *
     *  @param start 			starting node
     *  @param team 			winning team
     */
	void updateTree(node *start, int team);

	/*! @brief performs a playout of game and returns winner
     *
     *  @param node 			node with starting state of playout
     *  @return 	 			team value of winner 
     */
	int randPlayout(node *node);
};