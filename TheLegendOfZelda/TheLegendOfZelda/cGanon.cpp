#include "cGanon.h"

cGanon::cGanon() {}
cGanon::~cGanon() {}

void cGanon::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 32;
	float texture_size = 512;
	
	//hi ha 14 pixels en blanc que hem d'anar saltant
	if (GetState() == STATE_VISIBLE)
	{
		xo = ((float)84/512);	
		yo = ((float)312/512);

		xf = xo + (float)(sprite_size / texture_size);
		yf = yo + (float)(sprite_size / texture_size);

		DrawRect(tex_id, xo, yf, xf, yo);
	}
}

void cGanon::Logic(Tile *map) {
	int x0;
	int y0;

	GetPosition(&x0, &y0);

	int move = rand() % 200;
	if (move < 1) {
		int x = rand() % (10*TILE_SIZE);
		int y = rand() % (5*TILE_SIZE);
		SetPosition(x + 2*TILE_SIZE, y + 2*TILE_SIZE);
	}

	shooting--;
	if (shooting == 0) {
		throwProjectil = true;
		shooting = SHOOTING_WAIT;
	}

	if (visible > 0) visible--;
	else if (visible == 0) {
		visible = -1;
		SetState(STATE_INVISIBLE);
	}

	UpdateMapTiles(map, x0, y0);
	
}

cBicho* cGanon::ThrowProjectil(Tile* map) {
	if (throwProjectil) {
		cProjectil* projectil = new cProjectil();
		int x, y;
		GetPosition(&x, &y);
		projectil->SetPosition(x, y);
		projectil->SetState(rand()%4);
		projectil->SetWidthHeight(TILE_SIZE, TILE_SIZE);
		projectil->UpdateMapTiles(map, -1, -1);
		throwProjectil = false;
		return projectil;
	}
	else return nullptr;
}

bool cGanon::Blockable() {
	return false;
}

void cGanon::Hit(Tile* map) {
	if (visible == -1) {
		lifes--;
		if (lifes == 0) Destroy(map);
		else {
			visible = VISIBLE_TIME;
			SetState(STATE_VISIBLE);
		}
	}
}

void cGanon::UpdateMapTiles(Tile *map, int x0, int y0) {

	// Remove the bicho from original tiles
	if (x0 >= 0 && y0 >= 0 && y0 < TILE_SIZE * 11 && x0 < TILE_SIZE * 16) {
		std::vector<cBicho*> *bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
		RemoveBichoFromVector(bichos, this);
		bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE + 1].bichos;
		RemoveBichoFromVector(bichos, this);
		bichos = &map[(((y0 / TILE_SIZE) + 1)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
		RemoveBichoFromVector(bichos, this);
		bichos = &map[(((y0 / TILE_SIZE) + 1)*SCENE_WIDTH) + x0 / TILE_SIZE + 1].bichos;
		RemoveBichoFromVector(bichos, this);
	
		if (x0 % TILE_SIZE != 0 && x0 + TILE_SIZE < TILE_SIZE * 16) {
			bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE + 2].bichos;
			RemoveBichoFromVector(bichos, this);
			bichos = &map[(((y0 + 1) / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE + 2].bichos;
			RemoveBichoFromVector(bichos, this);
		}

		if (y0 % TILE_SIZE != 0 && y0 + TILE_SIZE < TILE_SIZE * 11) {
			bichos = &map[(((y0 / TILE_SIZE) + 2)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
			RemoveBichoFromVector(bichos, this);
			bichos = &map[(((y0 / TILE_SIZE) + 2)*SCENE_WIDTH) + x0 / TILE_SIZE + 1].bichos;
			RemoveBichoFromVector(bichos, this);
		}

		if (x0 % TILE_SIZE != 0 && y0 % TILE_SIZE != 0 && y0 + TILE_SIZE < TILE_SIZE * 11 && x0 + TILE_SIZE < TILE_SIZE * 16) {
			bichos = &map[(((y0 / TILE_SIZE) + 2)*SCENE_WIDTH) + x0 / TILE_SIZE + 2].bichos;
			RemoveBichoFromVector(bichos, this);
		}
	}

	// Add the bicho to the map with the new tiles
	int x, y;
	GetPosition(&x, &y);
	if (x != -1 && y != -1 && x < TILE_SIZE * 16 && y < TILE_SIZE * 11) {
		map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
		map[(((y + 1) / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
		map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		map[(((y + 1) / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		if (x % TILE_SIZE != 0 && x + TILE_SIZE < TILE_SIZE * 16) {
			map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.push_back(this);
			map[(((y + 1) / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.push_back(this);
		}
		if (y % TILE_SIZE != 0 && y + TILE_SIZE < TILE_SIZE * 11) {
			map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
			map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		}
		if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0 && x + TILE_SIZE < TILE_SIZE * 16 && y + TILE_SIZE < TILE_SIZE * 11)
			map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.push_back(this);
	}
}
