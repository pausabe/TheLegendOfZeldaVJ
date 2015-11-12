#pragma once

#include "Globals.h"

class cDungeon
{
protected:
	int currentDungeon = 0;

public:
	cDungeon();
	~cDungeon();

	void LoadEnemies(int dungeonId, vector<cBicho*> Enemies);
};
