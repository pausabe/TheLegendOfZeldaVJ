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

bool cGanon::isPreparing() {
	return preparing > -1;
}

void cGanon::Logic(Tile *map) {
	if (preparing > 0) {
		preparing--;
		return;
	}
	else if (preparing == 0) {
		SetState(STATE_INVISIBLE);
		preparing = -1;
	}

	int x0;
	int y0;

	GetPosition(&x0, &y0);

	if (visible == -1) {
		int move = rand() % 200;
		if (move < 1) {
			int x = rand() % (10 * TILE_SIZE);
			int y = rand() % (5 * TILE_SIZE);
			SetPosition(x + 2 * TILE_SIZE, y + 2 * TILE_SIZE);
		}

		shooting--;
		if (shooting == 0) {
			throwProjectil = true;
			shooting = SHOOTING_WAIT;
		}
	} 
	else if (visible == 0) {
		visible = -1;
		SetState(STATE_INVISIBLE);
		int x = rand() % (10 * TILE_SIZE);
		int y = rand() % (5 * TILE_SIZE);
		SetPosition(x + 2 * TILE_SIZE, y + 2 * TILE_SIZE);
	}
	else if (visible > 0) visible--;

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

void cGanon::Hit() {
	if (visible == -1) {
		lifes--;

		if (lifes == 0) {
			toBeDestroyed = true;
			PlaySound(TEXT("sounds/victory_FF.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
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
		if (map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos.size() == 0)
			map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);			
		std::vector<cBicho*>* bichos = &map[(((y / TILE_SIZE))*SCENE_WIDTH) + x / TILE_SIZE].bichos;
		
		if (map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos.size() == 0)
			map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
		bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
		
		if (map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.size() == 0)
			map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
		
		if (map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.size() == 0)
			map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
		
		if (x % TILE_SIZE != 0 && x + TILE_SIZE < TILE_SIZE * 16) {
			
			if (map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.size() == 0)
				map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.push_back(this);
			
			if (map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.size() == 0)
				map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.push_back(this);
		}
		if (y % TILE_SIZE != 0 && y + TILE_SIZE < TILE_SIZE * 11) {
			if (map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE].bichos.size() == 0)
				map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
			
			if (map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.size() == 0)
				map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		}
		if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0 && x + TILE_SIZE < TILE_SIZE * 16 && y + TILE_SIZE < TILE_SIZE * 11)
			if (map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.size() == 0)
				map[(((y / TILE_SIZE) + 2)*SCENE_WIDTH) + x / TILE_SIZE + 2].bichos.push_back(this);
	}
}

bool cGanon::ShieldBlocks(cBicho* bicho) {
	return false;
}

bool cGanon::isFlyer() {
	return true;
}