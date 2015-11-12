#pragma once

#include "cTerrestre.h"
#include "cGanon.h"
#include "cEspasa.h"
#include "cObject.h"
#include "cDungeon.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define JUMP_STEP			8
#define JUMP_LENGTH			TILE_SIZE*2
#define ATACK_DURATION		10
#define IMMUNITY_DURATION	100
#define STEP_LENGTH		4


class cPlayer: public cTerrestre
{
private:
	bool inDungeonTile = false;
	int jumping = -1;
	int atacking = -1;
	int jump;
	int immune = 0;

	std::vector<cObject*> HoldingObjects = std::vector<cObject*>();

public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	void Stop();
	void Logic(Tile* map, cDungeon Dungeon);
	// Dumb void logic
	void Logic(Tile* map);

	bool isJumping();
	void JumpBack(cRect* collider);
	void Hit(cRect* collider);

	cBicho* ThrowProjectil(Tile* map);

	int getAtacking();
	
	bool isImmune();

	void Atack(Tile* map);
	bool Blockable();

	void DetectAtackCollisions(cRect swordArea, std::vector<cBicho*> *bichos);
	bool ShieldBlocks(cBicho* bicho);

	void PickObject(cObject* o);
	bool HoldsKey(int key);
};
