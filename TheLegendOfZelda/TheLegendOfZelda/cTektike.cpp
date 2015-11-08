#include "cTektike.h"

cTektike::cTektike() {
	frameDelay = 16;
}
cTektike::~cTektike() {}

void cTektike::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	//hi ha 14 pixels en blanc que hem d'anar saltant
	switch (GetState())
	{
		//1
	case STATE_JUMPING:	
		xo = (float)((sprite_size + 14) * 8 / texture_size);	
		yo = (float)((sprite_size + 14) * 7 / texture_size);;
		break;
		//4
	case STATE_PREPARING:	
		xo = (float)((sprite_size + 14) * 8 / texture_size);	
		yo = (float)((sprite_size + 14) * 6 / texture_size);;
		break;
		//1..3
	case STATE_RESTING:	
		xo = (float)((sprite_size + 14) * 8 / texture_size);
		yo = (float)(sprite_size + 14) * 6 / texture_size + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;
	}


	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	DrawRect(tex_id, xo, yf, xf, yo);
}

void cTektike::Logic(Tile *map) {
	int x0;
	int y0;

	GetPosition(&x0, &y0);

	switch (GetState()) {
	case STATE_RESTING:
		if (rand() % JUMP_REPETITION < 1) {
			SetState(STATE_PREPARING);
			wait = PREPARATION_TIME;
			int length, degrees; 
			targetX = -1;
			targetY = -1;
			while (!(targetX >= 0 && targetY >= 0 && targetX < TILE_SIZE*16 && targetY < TILE_SIZE*11)) {
				length = rand() % JUMP_RADIUS;
				degrees = rand() % 360;
				targetX = length*cos(degrees*0.017453f) + x0;
				targetY = length*sin(degrees*0.017453f) + y0;
			}
			jumpY = y0;
			if (degrees < 90) degrees = 180 - degrees;
			if (degrees > 270) degrees = 180 + (360 - degrees);
			maxDegree = degrees;
			xStep = (targetX - x0) / (degrees / JUMP_STEP);
		}
		break;
	case STATE_PREPARING:
		if (wait == 0) {
			SetState(STATE_JUMPING);
			jump_alfa = 0;
		}
		else wait--;
		break;
	case STATE_JUMPING:
		int x, y;
		jump_alfa += JUMP_STEP;
		x = xStep + x0;
		float alfa = ((float)jump_alfa) * 0.017453f;
		y = jumpY + (int)(((float)64) * sin(alfa));

		if(jump_alfa >= maxDegree) {
			SetState(STATE_RESTING);
		}
		SetPosition(x, y);
		break;
	}
	UpdateMapTiles(map, x0, y0);
}

cBicho* cTektike::ThrowProjectil(Tile* map) {
	return nullptr;
}

bool cTektike::Blockable() {
	return false;
}