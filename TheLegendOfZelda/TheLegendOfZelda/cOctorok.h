#pragma once

#include "cTerrestre.h"
#include "cEnemy.h"
#include "cProjectil.h"

#define SHOOTING_TIME 32

class cOctorok : public cEnemy, public cTerrestre
{

private:
	int shooting = 0;

public:
	cOctorok();
	~cOctorok();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map); 
	bool Blockable();

};
