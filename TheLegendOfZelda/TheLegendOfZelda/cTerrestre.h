#pragma once

#include "cBicho.h"
#include "Globals.h"

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_WALKLEFT		2
#define STATE_WALKRIGHT		3
#define STATE_LOOKUP		4
#define STATE_LOOKDOWN		5
#define STATE_WALKUP		6
#define STATE_WALKDOWN		7


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

};
