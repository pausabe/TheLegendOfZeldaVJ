#pragma once

#include "cBicho.h"
#include <stdlib.h>

#define JUMP_RADIUS			TILE_SIZE*4
#define JUMP_STEP			8
#define PREPARATION_TIME	30
#define JUMP_REPETITION		20				// 1 time off of this number (JUMP_REPETITION == 100 -> jumps 1 of 100 times logic function is executed)

#define STATE_JUMPING	0
#define STATE_PREPARING 1
#define STATE_RESTING	2

class cTektike : public cBicho
{

private:
	int wait = 0;
	int jump_alfa = 0;
	int targetX, targetY;
	int xStep;
	int jumpY;
	int maxDegree;
	
public:
	cTektike();
	~cTektike();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map);
	bool Blockable();
	bool ShieldBlocks(cBicho* bicho);
};
