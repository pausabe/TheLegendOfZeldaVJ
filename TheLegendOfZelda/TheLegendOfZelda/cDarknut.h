#pragma once

#include "cTerrestre.h"
#include "cEnemy.h"

class cDarknut : public cEnemy, public cTerrestre
{

private:

public:
	cDarknut();
	~cDarknut();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map);
	bool Blockable();
	bool ShieldBlocks(cBicho* bicho);

};
