#pragma once

#include "Globals.h"
#include "cBicho.h"

#define STEP_LENGTH		4

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_LOOKUP		2
#define STATE_LOOKDOWN		3
#define STATE_WALKLEFT		4
#define STATE_WALKRIGHT		5
#define STATE_WALKUP		6
#define STATE_WALKDOWN		7

#define STATE_ATACKUP		8
#define STATE_ATACKDOWN		9
#define STATE_ATACKRIGHT	10
#define STATE_ATACKLEFT		11

class cTerrestre : public cBicho
{

protected:

	// True if the character is exactly in the tile coordinates
	bool inTileX = true;
	bool inTileY = true;
	// True if the character has collided with a wall tile
	bool collision = false;
	int move;
	int stepLength = STEP_LENGTH;

public:
	cTerrestre();
	~cTerrestre();

	// Returns true if the character collides with a map wall type tile in the specified direction
	bool CollidesMapTile(Tile* map, int direction);

	void Stop();

	void MoveRight(Tile *map);
	void MoveLeft(Tile *map);
	void MoveUp(Tile *map);
	void MoveDown(Tile *map);

	void SetCollision(bool collision);
	
	// Detects if the bicho collision is blocked by the self shield
	bool ShieldBlocks(cBicho* bicho);

};
