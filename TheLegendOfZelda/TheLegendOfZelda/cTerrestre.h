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
	bool CollidesMapTile(std::pair<int, bool>* map, int direction);

	void MoveRight(std::pair<int, bool> *map);
	void MoveLeft(std::pair<int, bool> *map);
	void MoveUp(std::pair<int, bool> *map);
	void MoveDown(std::pair<int, bool> *màp);

};
