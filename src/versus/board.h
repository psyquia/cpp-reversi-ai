#pragma once
#include <iostream>
#include <iomanip>      
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <utility>
#include <set>

#include "myhelper.h"


/*! coordinate to represent 2d positions
 */
typedef std::vector<int> coor;


/*! struct to hold information relevant to a valid move
 * contains position of valid move
 * and vector of positions to flip 
 */
struct flip_info{
	coor pos;
	std::vector<coor> to_flip;
};


/*! @brief game board implementation
 *
 * @details Features:
 *	- can print board
 *	- can add pieces to board
 *	- can get valid moves
 *	- can get scores
 *	- can get winner
 * 
 */
class Board 
{
public:
    Board();
    ~Board();

    /*! @brief prints the board and highlights valid positions
     *
     *  @param valid_moves      vector of valid moves
     */
	void printBoard(std::vector<flip_info> valid_moves={});


	/*! @brief given a command string representing a position (ex: b5)
     *		   places piece on board, and flips appropriate pieces
     *		   
     *  @param pos      		command string
     *  @param to_flip      	vector of positions to flip
     *  @param team      		team of function caller 
     */
	void doMove(std::string pos, std::vector<coor> to_flip, int team);


	/*! @brief like doMove except a coordinate is given instead of a command string
     *		   
     *  @param pos      		position coordinate of move
     *  @param to_flip      	vector of positions to flip
     *  @param team      		team of function caller 
     */
	void doMoveIndex(coor pos, std::vector<coor> to_flip, int team);


	/*! @brief given x and y, sets value of board position
     *		   
     *  @param x      			x value of pos
     *  @param y      			y value of pos
     *  @param val      		value
     */
	void setTile(int x, int y, int val);


	/*! @brief given x and y, returns value of board position
     *		   
     *  @param x      			char representing x value
     *  @param y      			y value of pos
     *  @return 	      		value
     */
	int getTile(char x, int y);


	/*! @brief given x and y, returns value of board position
     *		   
     *  @param x      			x value of pos
     *  @param y      			y value of pos
     *  @return 	      		value
     */
	int getTileFromIndex(int x, int y);


	/*! @brief return score of a team
     *		   
     *  @param team      		team 
     *  @return 	      		score (# of pieces of team)
     */
	int getScore(int team);


	/*! @brief return team with highest score
     *		   
     *  @return 	      		team with greater score
     */
	int getWinner();


	/*! @brief return vector of valid moves
     *		   
     *  @param team      		team 
     *  @param SWITCH_SIZE		# of pieces on the board at which getMoves() will change 
     *  						from early game search algorithm to late game algorithm
     *  
     *  @return 	      		team with greater score
     */
	std::vector<flip_info> getMoves(int team, int SWITCH_SIZE=15);


	/*! @brief for each team, a set of positions for their pieces on the board
     */
	std::set<coor> tiles[2];

private:

	/*! @brief board 
     */
	std::vector<int> board;


	/*! @brief team scores
     */
	std::vector<int> score;
};


/*! @brief tests a direction and returns positions to flip
 *		   
 *  @param b 	      		board
 *  @param xInitial			starting x value
 *  @param yInitial			starting y value
 *  @param direction		direction to search
 *  @param team				team
 */
static std::vector<coor> checkDirection(Board &b, int xInitial,int yInitial,
									coor direction, int team);


/*! @brief getMoves algorithm for early game
 *		   
 *	@details searches starting from pieces already on the board instead of starting
 *		     search from empty positions
 *		   
 *  @param b 	      		board
 *  @param res				result containing valid moves
 *  @param team				team
 */
static void getMoves_early(Board &b, std::vector<flip_info> &res, int team);


/*! @brief (early game algorithm) tests a direction and appends valid moves to map
 *		   
 *  @param b 	      		board
 *  @param valid_tiles		map with valid moves (key: position, value: positions to flip) 
 *  @param xInitial			starting x value
 *  @param yInitial			starting y value
 *  @param direction		direction to search
 *  @param team				team
 */
static void checkDir_early(Board &b, std::map<coor,std::vector<coor>> &valid_tiles, 
							int xInitial, int yInitial, coor direction, int team);





