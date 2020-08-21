#pragma once
#include "board.h"

using namespace std;

/*! @brief state of the game
 *
 * @details represents the state of the game at any point
 * 
 */
class State {
public:

	/*! @brief game board of state
	 */
	Board board;

	/*! @brief move that led to state
	 */
	flip_info move;
	
	bool game_over=false;
	int team;
	int score;

	State();
	~State();

	/*! @brief construct state from board and team
     *
     *  @param b      			board
     *  @param team_p      		team
     */
	State(Board b, int team_p);


	/*! @brief construct state from board, team, and move
     *
     *  @param b      			board
     *  @param mov      		move that lead to the state
     *  @param team_p      		team
     */
	State(Board b, flip_info mov, int team_p);


	/*! @brief returns child states
     *
     *  @return      			vector of child states
     */
	vector<State> getChildStates();


	/*! @brief returns winner
     *
     *  @return      			winner
     */
	int getWinner();


	/*! @brief alters state by performing a random move
     *
     */
	void doRandMove();
};

struct node 
{
	State state;
	node *parent;
	int visits=0;
	vector<node*> children;
	node();
	~node();
	node(State s);
	node(State s, node *p);
};