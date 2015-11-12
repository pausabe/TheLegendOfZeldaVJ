#include "cDungeon.h"

cDungeon::cDungeon() {

	// Initialize enemies vector

}
cDungeon::~cDungeon() {}

void cDungeon::LoadEnemies(int dungeonId, std::vector<cBicho*>* Enemies) {
	switch (dungeonId) {
	case 1:
		break;

	case 2:
		break;
	}

	Enemies = &dungeonEnemies[dungeonId];
}