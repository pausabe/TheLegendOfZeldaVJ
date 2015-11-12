#pragma once

#include "cProjectil.h"
#include "cGanon.h"
#define SPEED 10
#define explosionDelay 10


class cEspasa : public cProjectil
{
public:
	cEspasa();
	~cEspasa();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map);

	bool Blockable();
	
	bool deadSword = false;

	int timeExplosion = 0;
	bool ShieldBlocks(cBicho* bicho);
	void DetectAtackCollisions(std::vector<cBicho*> *bichos);

};
