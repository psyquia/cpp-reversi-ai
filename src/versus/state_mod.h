#pragma once
#include <climits>
#include "board.h"

using namespace std;

/*! @brief positional value for each position tier
 *
 * @details
 * 
 *	good positions (@see img/goodspots.jpg)
 *	 - three Ts of good positions
 *	 	(1) corners: best pos (gold check in illustration)
 *	 	(2) positions controlling corners (green check)
 *	 	(3) positions controlling (2) (blue check)
 *	 - the higher the T, the greater the score
 */
#define T1 5
#define T2 3
#define T3 2

/*! @brief state of the game
 *
 * @details represents the state of the game at any point
 * 
 */
class State_mod {
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

	State_mod();
	~State_mod();

	/*! @brief construct state from board and team
     *
     *  @param b      			board
     *  @param team_p      		team
     */
	State_mod(Board b, int team_p);


	/*! @brief construct state from board, team, and move
     *
     *  @param b      			board
     *  @param mov      		move that lead to the state
     *  @param team_p      		team
     */
	State_mod(Board b, flip_info mov, int team_p);


	/*! @brief returns child states
     *
     *  @return      			vector of child states
     */
	vector<State_mod> getChildStates();


	/*! @brief returns winner
     *
     *  @return      			winner
     */
	int getWinner();


	/*! @brief alters state by performing a move according to some heuristics
     *
     *	@details decides based on the sum of capture positional values
     *	 - the move that produces the greatest sum is chosen
     *
     *	bad positions (@see img/badspots.jpg)
     *	 - positions directly adjacent to corners
     *	 - given a negative score
     *
     *	good positions (@see img/goodspots.jpg)
     *	 - three Ts of good positions
     *	 	(1) corners: best pos (gold check in illustration)
     *	 	(2) positions controlling corners (green check)
     *	 	(3) positions controlling (2) (blue check)
     *	 - the higher the T, the greater the score
     *
     */
	void doMove();
};

struct node_mod 
{
	State_mod state;
	node_mod *parent;
	int visits=0;
	vector<node_mod*> children;
	node_mod();
	~node_mod();
	node_mod(State_mod s);
	node_mod(State_mod s, node_mod *p);
};