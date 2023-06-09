#ifndef GAME_MAP_H
#define GAME_MAP_H

#define MAX_TILES 20

#include "CommonFunc.h"
#include "BaseObject.h"

class TileMat : public BaseObject
{
public:
	TileMat() {;}
	~TileMat() {;}
};

class GameMap
{
public:
	GameMap() {;}
	~GameMap() {;}

	void LoadMap(const char* name); // doc map.dat
	void LoadTiles(SDL_Renderer* screen, const int& idx); // load anh cua tung o tile
	void DrawMap(SDL_Renderer* screen); 
	Map getMap() const { return game_map_; }
	void SetMap(Map& map_data) { game_map_ = map_data; }


private:
	Map game_map_;
	TileMat tile_mat_[MAX_TILES];
};

#endif