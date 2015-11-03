#pragma once

#include "cTerrestre.h"
#include "cEnemy.h"
#include "cProjectil.h"

class cOctorok : public cEnemy, public cTerrestre
{
public:
	cOctorok();
	~cOctorok();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map); 

};
