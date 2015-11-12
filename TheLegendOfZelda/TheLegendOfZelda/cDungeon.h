#pragma once

#include "Globals.h"


class cDungeon
{
protected:
	int currentDungeon = 0;

	std::vector<cBicho*>* dungeonEnemies[8];

public:
	cDungeon();
	~cDungeon();

	void LoadEnemies(int dungeonId, std::vector<cBicho*>* Enemies);
};
