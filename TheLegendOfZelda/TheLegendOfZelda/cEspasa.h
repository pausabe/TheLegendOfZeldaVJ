#pragma once

#include "cProjectil.h"
#define SPEED 16


class cEspasa : public cProjectil
{
public:
	cEspasa();
	~cEspasa();

	void Draw(int tex_id);
	void Logic(Tile *map);
	cBicho* ThrowProjectil(Tile* map);

	bool Blockable();

};
