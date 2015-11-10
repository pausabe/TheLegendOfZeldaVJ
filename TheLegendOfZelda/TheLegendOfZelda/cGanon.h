#pragma once

#include "cTerrestre.h"
#include "cEnemy.h"
#include "cProjectil.h"

#define VISIBLE_TIME 16
#define SHOOTING_WAIT 32
#define STATE_INVISIBLE 0
#define STATE_VISIBLE 0


class cGanon : public cEnemy, public cTerrestre
{

private:
	int shooting = SHOOTING_WAIT;
	int visible = -1;

public:
	cGanon();
	~cGanon();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map);
	bool Blockable();

	void Hit(Tile* map);
	void UpdateMapTiles(Tile *map, int x0, int y0);
};
