#pragma once

#include "cTerrestre.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define JUMP_STEP			8
#define JUMP_LENGTH			TILE_SIZE*2

class cPlayer: public cTerrestre
{
private:
	int jumping = -1;
	int jump;
public:
	cPlayer();
	~cPlayer();

	void SetPosition(int x, int y, Tile* map);
	void SetTile(int tx, int ty, Tile* map);
	void Draw(int tex_id);
	void Logic(Tile *map);
	bool isJumping();
	void JumpBack(cRect* collider);
	cBicho* ThrowProjectil(Tile* map);
};
