#pragma once

#include "cTerrestre.h"

class cEnemy
{
public:
	cEnemy();
	~cEnemy();

	virtual bool Blockable() = 0;
};
