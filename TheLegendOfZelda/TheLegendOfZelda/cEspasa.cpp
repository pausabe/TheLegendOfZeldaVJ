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

	float xxx = 30;
	float yyy = 194;

	xo = (float)(xxx / texture_size);
	yo = ((float)(yyy / texture_size));
	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	DrawRect(tex_id, xo, yf, xf, yo);
}

void cEspasa::Logic(Tile *map) {
	int x0;
	int y0;

	GetPosition(&x0, &y0);
	if (x0 < 0 || y0 < 0 || collision) Destroy(map);
	if (GetState() == STATE_LOOKLEFT || GetState() == STATE_WALKLEFT || GetState() == STATE_ATACKLEFT) MoveLeft(map);
	else if (GetState() == STATE_LOOKRIGHT || GetState() == STATE_WALKRIGHT || GetState() == STATE_ATACKRIGHT) MoveRight(map);
	else if (GetState() == STATE_LOOKUP || GetState() == STATE_WALKUP || GetState() == STATE_ATACKUP) MoveUp(map);
	else if (GetState() == STATE_LOOKDOWN || GetState() == STATE_WALKDOWN || GetState() == STATE_ATACKDOWN) MoveDown(map);

}

cBicho* cEspasa::ThrowProjectil(Tile* map) {
	return nullptr;
}

bool cEspasa::Blockable() {
	return true;
}