#pragma once

#include "cTerrestre.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

class cPlayer: public cTerrestre
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
};
