#pragma once

#include "cPlayer.h"

#define TIME 50*10

class cTes : public cPlayer {
private:
	int BFS(Tile* map);
	int time = -1;
	int move;

public:
	cTes();
	~cTes();

	void Draw(int tex_id);
	void Logic(Tile* map);
	void Logic(Tile* map, std::vector<cBicho*> Enemies);
};