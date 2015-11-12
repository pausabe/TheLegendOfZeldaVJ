#include "cDungeon.h"

cDungeon::cDungeon() {
	// Initialize objects vector
	dungeonObjects[0] = new std::vector<cObject*>();
	dungeonObjects[1] = new std::vector<cObject*>();
	dungeonObjects[2] = new std::vector<cObject*>();
	dungeonObjects[3] = new std::vector<cObject*>();
	dungeonObjects[4] = new std::vector<cObject*>();
	dungeonObjects[5] = new std::vector<cObject*>();
	dungeonObjects[6] = new std::vector<cObject*>();
	dungeonObjects[7] = new std::vector<cObject*>();

	cStepladder* Stepladder = new cStepladder();
	Stepladder->SetPosition(4 * TILE_SIZE, 5 * TILE_SIZE);
	dungeonObjects[4]->push_back(Stepladder);

	// Initialize enemies vector
	dungeonEnemies[0] = new std::vector<cBicho*>();
	dungeonEnemies[1] = new std::vector<cBicho*>();
	dungeonEnemies[2] = new std::vector<cBicho*>();
	dungeonEnemies[3] = new std::vector<cBicho*>();
	dungeonEnemies[4] = new std::vector<cBicho*>();
	dungeonEnemies[5] = new std::vector<cBicho*>();
	dungeonEnemies[6] = new std::vector<cBicho*>();
	dungeonEnemies[7] = new std::vector<cBicho*>();

	// Room 2

	cOctorok* o1 = new cOctorok();
	o1->SetTile(4, 5);
	o1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o1->SetState(STATE_LOOKDOWN);
	o1->SetLifes(1);
	cOctorok* o2 = new cOctorok();
	o2->SetTile(3, 3);
	o2->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o2->SetState(STATE_LOOKDOWN);
	o2->SetLifes(1);
	cOctorok* o3 = new cOctorok();
	o3->SetTile(6, 3);
	o3->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o3->SetState(STATE_LOOKDOWN);
	o3->SetLifes(1);
	cOctorok* o4 = new cOctorok();
	o4->SetTile(10, 7);
	o4->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o4->SetState(STATE_LOOKDOWN);
	o4->SetLifes(1);

	dungeonEnemies[1]->push_back(o1);
	dungeonEnemies[1]->push_back(o2);
	dungeonEnemies[1]->push_back(o3);
	dungeonEnemies[1]->push_back(o4);

	// Room 3

	cTektike* d1 = new cTektike();
	d1->SetTile(13, 7);
	d1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d1->SetState(STATE_RESTING);
	d1->SetLifes(1);
	cTektike* d2 = new cTektike();
	d2->SetTile(3, 7);
	d2->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d2->SetState(STATE_RESTING);
	d2->SetLifes(1);
	cTektike* d3 = new cTektike();
	d3->SetTile(7, 3);
	d3->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d3->SetState(STATE_RESTING);
	d3->SetLifes(1);
	cTektike* d4 = new cTektike();
	d4->SetTile(6, 5);
	d4->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d4->SetState(STATE_RESTING);
	d4->SetLifes(1);
	cTektike* d5 = new cTektike();
	d5->SetTile(10, 5);
	d5->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d5->SetState(STATE_RESTING);
	d5->SetLifes(1);

	cDarknut* d = new cDarknut();
	d->SetTile(3, 7);
	d->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d->SetState(STATE_LOOKDOWN);
	d->SetLifes(4);
	cKey* k = new cKey();
	k->SetKeyId(1);
	d->SetDroppingObject(k);

	dungeonEnemies[2]->push_back(d1);
	dungeonEnemies[2]->push_back(d2);
	dungeonEnemies[2]->push_back(d3);
	dungeonEnemies[2]->push_back(d4);
	dungeonEnemies[2]->push_back(d5);
	dungeonEnemies[2]->push_back(d);

	// Room 4

	cDarknut* x1 = new cDarknut();
	x1->SetTile(5, 7);
	x1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	x1->SetState(STATE_LOOKDOWN);
	x1->SetLifes(4);
	cTektike* x2 = new cTektike();
	x2->SetTile(10, 5);
	x2->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	x2->SetState(STATE_RESTING);
	x2->SetLifes(1);
	cTektike* x3 = new cTektike();
	x3->SetTile(3, 4);
	x3->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	x3->SetState(STATE_RESTING);
	x3->SetLifes(1);
	cOctorok* x4 = new cOctorok();
	x4->SetTile(6, 4);
	x4->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	x4->SetState(STATE_LOOKDOWN);
	x4->SetLifes(1);
	cOctorok* x5 = new cOctorok();
	x5->SetTile(12, 4);
	x5->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	x5->SetState(STATE_LOOKDOWN);
	x5->SetLifes(1);

	dungeonEnemies[3]->push_back(x1);
	dungeonEnemies[3]->push_back(x2);
	dungeonEnemies[3]->push_back(x3);
	dungeonEnemies[3]->push_back(x4);
	dungeonEnemies[3]->push_back(x5);

	// Room 7

	cDarknut* c1 = new cDarknut();
	c1->SetTile(5, 3);
	c1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c1->SetState(STATE_LOOKDOWN);
	c1->SetLifes(4);
	cDarknut* c2 = new cDarknut();
	c2->SetTile(3, 7);
	c2->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c2->SetState(STATE_LOOKDOWN);
	c2->SetLifes(4);
	cDarknut* c3 = new cDarknut();
	c3->SetTile(4, 5);
	c3->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c3->SetState(STATE_LOOKDOWN);
	c3->SetLifes(4);
	cKey* k1 = new cKey();
	k1->SetKeyId(2);
	c3->SetDroppingObject(k1);

	dungeonEnemies[6]->push_back(c1);
	dungeonEnemies[6]->push_back(c2);
	dungeonEnemies[6]->push_back(c3);

	// Room 8 -> Ganondorf!

	cGanon* g = new cGanon();
	g->SetTile(4, 3);
	g->SetWidthHeight(TILE_SIZE*2, TILE_SIZE*2);
	g->SetState(STATE_VISIBLE);
	g->SetLifes(8);

	dungeonEnemies[7]->push_back(g);

}
cDungeon::~cDungeon() {}

void cDungeon::LoadEnemies(int dungeonId, std::vector<cBicho*>* Enemies) {	
	currentDungeon = dungeonId;
	Enemies->assign(dungeonEnemies[dungeonId]->begin(), dungeonEnemies[dungeonId]->end());
}

void cDungeon::LoadObjects(int dungeonId, std::vector<cObject*>* Objects) {
	currentDungeon = dungeonId;
	Objects->assign(dungeonObjects[dungeonId]->begin(), dungeonObjects[dungeonId]->end());
}

void cDungeon::ExitDungeon() {
	currentDungeon = -1;
}

int cDungeon::GetCurrentDungeon() {
	return currentDungeon;
}

void cDungeon::OpenDoor(int door) {
	switch (door) {
	case 1:
		door1 = true;
		break;
	case 2:
		door2 = true;
		break;
	case 3:
		door3 = true;
		break;
	}
}

bool cDungeon::isOpen(int door) {
	switch (door) {
	case 1:
		return door1;
	case 2:
		return door2;
		break;
	case 3:
		return door2;
	}
	return false;
}

bool cDungeon::EnemiesCleared() {
	for (int i = 0; i < dungeonEnemies[1]->size(); i++) {
		if ((*dungeonEnemies[1])[i]->GetPosX() != -1) return false;
	}
	for (int i = 0; i < dungeonEnemies[2]->size(); i++) {
		if ((*dungeonEnemies[2])[i]->GetPosX() != -1) return false;
	}
	for (int i = 0; i < dungeonEnemies[3]->size(); i++) {
		if ((*dungeonEnemies[3])[i]->GetPosX() != -1) return false;
	}
	for (int i = 0; i < dungeonEnemies[5]->size(); i++) {
		if ((*dungeonEnemies[5])[i]->GetPosX() != -1) return false;
	}
	return true;
}