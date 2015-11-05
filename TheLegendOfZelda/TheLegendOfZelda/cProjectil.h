#pragma once

#include "cTerrestre.h"
#include "cEnemy.h"

#define SPEED 16

class cProjectil : public cEnemy, public cTerrestre
{
public:
	cProjectil();
	~cProjectil();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map);

	bool Blockable();

};
