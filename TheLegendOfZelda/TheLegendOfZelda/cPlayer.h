#pragma once

#include "cTerrestre.h"
#include "cEspasa.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define JUMP_STEP			8
#define JUMP_LENGTH			TILE_SIZE*2
#define ATACK_DURATION		10
#define IMMUNITY_DURATION	100


class cPlayer: public cTerrestre
{
private:
	int jumping = -1;
	int atacking = -1;
	int jump;
	int immune = 0;
public:
	cPlayer();
	~cPlayer();

	void SetPosition(int x, int y, Tile* map);
	void SetTile(int tx, int ty, Tile* map);
	void Draw(int tex_id);
	void Stop();
	void Logic(Tile *map);

	bool isJumping();
	void JumpBack(cRect* collider);
	void Hit(cRect* collider);

	cBicho* ThrowProjectil(Tile* map);

	int getAtacking();
	
	bool isImmune();

	void Atack(Tile * map);
	bool Blockable();

};
