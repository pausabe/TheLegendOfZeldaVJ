#pragma once

#include "Globals.h"
#include "cBicho.h"
#include "cTektike.h"
#include "cDarknut.h"
#include "cGanon.h"
#include "cOctorok.h"
#include "cKey.h"
#include "cStepladder.h"

class cDungeon
{
protected:
	int currentDungeon = -1;
	bool door1 = false, door2 = false, door3 = false; // True if the doors are open. door1 -> room 2, door2 -> room 6 left, door3 -> room 6 top
	std::vector<cBicho*>* dungeonEnemies[8];
	std::vector<cObject*>* dungeonObjects[8];
	cStepladder Stepladder;

public:
	cDungeon();
	~cDungeon();

	void LoadEnemies(int dungeonId, std::vector<cBicho*>* Enemies);
	void LoadObjects(int dungeonId, std::vector<cObject*>* Objects);
	void ExitDungeon();
	int GetCurrentDungeon();
	void OpenDoor(int door);
	bool isOpen(int door);
};
