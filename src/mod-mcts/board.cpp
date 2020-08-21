#include "board.h"

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------


Board::Board(){
	board = vector<int>(64,-1);
	score = vector<int>(2,2);

	tiles[0] = {{3,3},	{4,4}};
	tiles[1] = {{3,4},	{4,3}};

	for(auto pos : tiles[1]){
		this->setTile(pos[0], pos[1], 1);
		this->tiles[1].insert({pos[0],pos[1]});
	}

	for(auto pos : tiles[0]){
		this->setTile(pos[0], pos[1], 0);
		this->tiles[0].insert({pos[0],pos[1]});
	}
}

// ---------------------------------------------------------------------------------------------------------------------


Board::~Board(){

}

// ---------------------------------------------------------------------------------------------------------------------


int Board::getTile(char x, int y){
	return this->board[8*(y-1) + index_map[x]-1];
}

// ---------------------------------------------------------------------------------------------------------------------


int Board::getTileFromIndex(int x, int y){
	return this->board[8*(y) + x];
}

// ---------------------------------------------------------------------------------------------------------------------


int Board::getScore(int team){
	return this->score[team];
}

// ---------------------------------------------------------------------------------------------------------------------


/*
Return Value:
	1 if X win
	0 if O win
	-1 if draw
*/
int Board::getWinner(){
	if(score[0]==score[1])
		return -1;
	return (score[0] > score[1]) ? 0 : 1;
}

// ---------------------------------------------------------------------------------------------------------------------


void Board::doMove(string pos, vector<coor> to_flip, int team){
	this->setTile(index_map[pos[0]]-1, stoi(pos.substr(1,1),nullptr,10)-1, team);
	this->tiles[team].insert({index_map[pos[0]]-1,stoi(pos.substr(1,1),nullptr,10)-1});
	this->score[team]++;
	for(auto tile : to_flip){
		this->setTile(tile[0], tile[1], team);
		this->tiles[team].insert({tile[0],tile[1]});
		this->tiles[!team].erase({tile[0],tile[1]});
		this->score[team]++;
		this->score[!team]--;
	}
}

// ---------------------------------------------------------------------------------------------------------------------


void Board::doMoveIndex(coor pos, vector<coor> to_flip, int team){
	this->setTile(pos[0], pos[1], team);
	this->tiles[team].insert({pos[0],pos[1]});
	this->score[team]++;
	for(auto tile : to_flip){
		this->setTile(tile[0], tile[1], team);
		this->tiles[team].insert({tile[0],tile[1]});
		this->tiles[!team].erase({tile[0],tile[1]});
		this->score[team]++;
		this->score[!team]--;
	}
}

// ---------------------------------------------------------------------------------------------------------------------


void Board::setTile(int x, int y, int val){
	this->board[8*(y) + x] = val;
}

// ---------------------------------------------------------------------------------------------------------------------


void Board::printBoard(vector<flip_info> valid_moves){

	const char *black_s = u8"○";
	const char *white_s = u8"●";
	const char *valid_s = "*";
	const char *empty_s = " ";


	set<coor> valid_pos = {};

	for(auto valid : valid_moves){
		valid_pos.insert(valid.pos);
	}

	const char *c;
	int val;
	cout << "   ---------------------------------" << endl;
	for(int i=7; i>=0; i--){
		cout << " " << i+1;
		for(int j=0; j<8; j++){
			val = this->board[8*i + j];
			if(valid_pos.count({j,i})){
				cout << " | " << valid_s;
			}else if(val == 0){
				cout << " | " << white_s;
			}else if(val == 1){
				cout << " | " << black_s;
			}else
				cout << " | " << empty_s;
		}
		cout << " | " << endl;
		cout << "   ---------------------------------" << endl;
	}
	cout << "     A   B   C   D   E   F   G   H  " << endl;

}

// ---------------------------------------------------------------------------------------------------------------------


vector<flip_info> Board::getMoves(int team, int SWITCH_SIZE){ 
	vector<flip_info> res = {};

	// If this team occupies more than half of the board;
	if(this->tiles[team].size() > SWITCH_SIZE){
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				if(this->getTileFromIndex(i,j) != -1){
					continue;
				}

				flip_info newValidTile;
				newValidTile.pos = {i,j};
				newValidTile.to_flip = {};

				for(auto dir : directions){
					auto tmp = checkDirection(*this, i,j,dir,team);
					if(tmp.size() > 0){
						newValidTile.to_flip.insert(newValidTile.to_flip.end(),
													tmp.begin(), tmp.end());
					}
				}

				if(newValidTile.to_flip.size() > 0){
					res.push_back(newValidTile);
				}
			}
		}
	}else{
		getMoves_early(*this, res, team);
	}
	

	return res;
}

// ---------------------------------------------------------------------------------------------------------------------


// Fills @param res
// Searches for valid positions starting from occupied tiles
static void getMoves_early(Board &b, vector<flip_info> &res, int team){
	map<coor,vector<coor>> valid_tiles;
	int count = 0;

	for(auto tile : b.tiles[team]){
		count++;
		for(auto dir : directions){
			checkDir_early(b,valid_tiles,tile[0],tile[1],dir,team);
		}
	}

	for(auto &tile : valid_tiles){
		flip_info fValid;
		fValid.pos = tile.first;
		fValid.to_flip = tile.second;
		res.push_back(fValid);
	}

}

// ---------------------------------------------------------------------------------------------------------------------


static void checkDir_early(Board &b, map<coor,vector<coor>> &valid_tiles, int xInitial, int yInitial, coor direction, int team){
	int x = xInitial, y = yInitial;
	int xd = direction[0];
	int yd = direction[1];

	coor end_tile;

	// Check adjacent tile
	x += xd;
	y += yd;

	// out of bounds
	if(x >= 8 || y >= 8 || x < 0 || y < 0){
		return;
	}

	// Adjacent tile is not enemy tile -> abort
	if(b.getTileFromIndex(x,y) != !team){
		return;
	}

	vector<coor> path = {};
	
	path.push_back({x,y});

	while(1){
		x += xd;
		y += yd;

		if(x >= 8 || y >= 8 || x < 0 || y < 0){
			return;
		}

		if(b.getTileFromIndex(x,y) == team){
			return;
		}

		if(b.getTileFromIndex(x,y) == -1){
			end_tile = {x,y};
			break;
		}

		path.push_back({x,y});
	}

	auto validI = valid_tiles.find(end_tile);

	// Check if end tile is already in set
	if(validI != valid_tiles.end()){
		validI->second.insert(validI->second.end(),path.begin(),path.end());
	}else{
		valid_tiles.insert(pair<coor,vector<coor>>{end_tile,path});
	}
 
	return;
}

// ---------------------------------------------------------------------------------------------------------------------


static vector<coor> checkDirection(Board &b, int xInitial, int yInitial, coor direction, int team){
	vector<coor> res = {};
	int x = xInitial, y = yInitial;
	int xd = direction[0];
	int yd = direction[1];
	while(1){
		x += xd;
		y += yd;

		if(x >= 8 || y >= 8 || x < 0 || y < 0){
			return {};
		}

		if(b.getTileFromIndex(x,y) == -1){
			return {};
		}

		if(b.getTileFromIndex(x,y) == team){
			break;
		}

		res.push_back({x,y});

	}
	return res;
}



