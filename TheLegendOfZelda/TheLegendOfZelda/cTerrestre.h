#pragma once

#include "cBicho.h"
#include "Globals.h"

class cTerrestre : public cBicho
{

protected:

	// Is true if the character is exactly in the tile coordinates
	bool inTileX, inTileY;
	int move;

public:
	cTerrestre();
	~cTerrestre();

	// Returns true if the character collides with a map wall type tile in the specified direction
	// Directions: 0 = left, 1 = top, 2 = right, 3 = bottom
	bool CollidesMapTile(Tile* map, int direction);

	void MoveRight(Tile *map);
	void MoveLeft(Tile *map);
	void MoveUp(Tile *map);
	void MoveDown(Tile *màp);

};
