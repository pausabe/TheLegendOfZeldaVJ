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

	if (!deadSword) {
		float xxx, yyy;
		yyy = 194;

		int state = GetState();

		if(state== STATE_ATACKDOWN || state == STATE_LOOKDOWN || state == STATE_WALKDOWN) xxx = 0;
		else if (state == STATE_ATACKUP || state == STATE_LOOKUP || state == STATE_WALKUP) xxx = 60;
		else if (state == STATE_ATACKRIGHT || state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT) xxx = 90;
		else if (state == STATE_ATACKLEFT || state == STATE_LOOKLEFT || state == STATE_WALKLEFT) xxx = 30;

		xo = (float)(xxx / texture_size);
		yo = ((float)(yyy / texture_size));
		xf = xo + (float)(sprite_size / texture_size);
		yf = yo + (float)(sprite_size / texture_size);

		DrawRect(tex_id, xo, yf, xf, yo);
	}
	else {
		float xxx = 178;
		float yyy = 282;

		float xxx2 = 196;
		float yyy2 = 302;

		xo = (float)(xxx / texture_size);
		yo = ((float)(yyy / texture_size));
		xf = (float)(xxx2/ texture_size);
		yf = (float)(yyy2/ texture_size);

		DrawRect(tex_id, xo, yf, xf, yo);
	}
}

void cEspasa::Logic(Tile *map) {
	int x0;
	int y0;

	GetPosition(&x0, &y0);
	if (x0 < 0 || y0 < 0 || collision) {
		if (!deadSword) deadSword = true;
		else {
			timeExplosion++;
			if (timeExplosion == explosionDelay) toBeDestroyed = true;
		}
	}
	else if (GetState() == STATE_LOOKLEFT || GetState() == STATE_WALKLEFT || GetState() == STATE_ATACKLEFT) MoveLeft(map);
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