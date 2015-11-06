#include "cEspasa.h"

cEspasa::cEspasa() {
	stepLength = SPEED;
}
cEspasa::~cEspasa() {}

void cEspasa::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	xo = (float)((sprite_size + 14) * 11 / texture_size);
	yo = (float)0;
	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	DrawRect(tex_id, xo, yf, xf, yo);
}

void cEspasa::Logic(Tile *map) {
	int x0;
	int y0;

	GetPosition(&x0, &y0);
	if (x0 < 0 || y0 < 0 || collision) toBeDestroyed = true;
	if (GetState() == STATE_LOOKLEFT || GetState() == STATE_WALKLEFT) MoveLeft(map);
	else if (GetState() == STATE_LOOKRIGHT || GetState() == STATE_WALKRIGHT) MoveRight(map);
	else if (GetState() == STATE_LOOKUP || GetState() == STATE_WALKUP) MoveUp(map);
	else if (GetState() == STATE_LOOKDOWN || GetState() == STATE_WALKDOWN) MoveDown(map);

}

cBicho* cEspasa::ThrowProjectil(Tile* map) {
	return nullptr;
}

bool cEspasa::Blockable() {
	return false;
}