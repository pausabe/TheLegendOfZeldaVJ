#pragma once

#include "cBicho.h"
#include "Globals.h"

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

	void MoveRight(Tile *map);
	void MoveLeft(Tile *map);
	void MoveUp(Tile *map);
	void MoveDown(Tile *màp);

};
