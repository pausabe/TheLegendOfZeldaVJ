#pragma once

#include "cTerrestre.h"
#include "cEnemy.h"

class cOctorok : public cEnemy, public cTerrestre
{
public:
	cOctorok();
	~cOctorok();

	void Draw(int tex_id);
	void Logic(std::pair<int, bool> *map);

};
