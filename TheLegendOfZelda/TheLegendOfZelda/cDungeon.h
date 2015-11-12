#pragma once

#include "Globals.h"
#include "cOctorok.h"
#include "cTektike.h"
#include "cDarknut.h"
#include "cGanon.h"
#include "cKey.h"
#include "cStepladder.h"


class cDungeon
{
protected:
	int currentDungeon = 0;

	std::vector<cBicho*>* dungeonEnemies[8]; // = { &std::vector<cBicho*>(),&std::vector<cBicho*>(),
		//&std::vector<cBicho*>(),&std::vector<cBicho*>(),&std::vector<cBicho*>(),
		//&std::vector<cBicho*>(),&std::vector<cBicho*>(),&std::vector<cBicho*>() };

public:
	cDungeon();
	~cDungeon();

	void LoadEnemies(int dungeonId, std::vector<cBicho*>* Enemies);
};
