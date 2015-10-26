#pragma once

#include "cBicho.h"
#include "Globals.h"

class cTerrestre : public cBicho
{
public:
	cTerrestre();
	~cTerrestre();

	// Returns true if the character collides with a map wall type tile in the specified direction
	// Directions: 0 = left, 1 = top, 2 = right, 3 = bottom
	bool CollidesMapTile(int* map, int direction);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int *map);
	void MoveDown(int *màp);

};
