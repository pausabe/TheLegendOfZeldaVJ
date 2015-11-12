#include "cDungeon.h"

cDungeon::cDungeon() {



	// Initialize enemies vector
	cOctorok* c = new cOctorok();
	c->SetTile(4, 5);
	c->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c->SetState(STATE_LOOKDOWN);

	dungeonEnemies[0].push_back(c);
}
cDungeon::~cDungeon() {}

void cDungeon::LoadEnemies(int dungeonId, std::vector<cBicho*>* Enemies, Tile* map) {
	switch (dungeonId) {
	case 1:
		break;

	case 2:
		break;
	}

	Enemies = &dungeonEnemies[dungeonId];
}