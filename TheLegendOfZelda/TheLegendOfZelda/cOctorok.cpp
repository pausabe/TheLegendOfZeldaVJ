#include "cOctorok.h"

cOctorok::cOctorok() {}
cOctorok::~cOctorok() {}

void cOctorok::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	//hi ha 14 pixels en blanc que hem d'anar saltant
	switch (GetState())
	{
		//1
	case STATE_LOOKLEFT:	xo = (float)((sprite_size + 14) / texture_size);	yo = 0.0f;
		break;
		//4
	case STATE_LOOKRIGHT:	xo = (float)((sprite_size + 14) / texture_size) * 3;	yo = 0.0f;
		break;
		//1..3
	case STATE_WALKLEFT:	xo = (float)((sprite_size + 14) / texture_size);
		yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;
		//4..6
	case STATE_WALKRIGHT:	xo = (float)((sprite_size + 14) / texture_size) * 3;
		yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;

	case STATE_LOOKUP:		xo = (float)((sprite_size + 14) / texture_size) * 2;	yo = 0.0f;
		break;

	case STATE_LOOKDOWN:	xo = 0.0f;	yo = 0.0f;
		break;

	case STATE_WALKUP:		xo = (float)((sprite_size + 14) / texture_size) * 2;
		yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;

	case STATE_WALKDOWN:	xo = 0.0f;
		yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;
	}


	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	DrawRect(tex_id, xo, yf, xf, yo);
}

void cOctorok::Logic(Tile *map) {
	if (shooting <= 0) {
		int x0;
		int y0;

		GetPosition(&x0, &y0);

		int shoot = rand() % 400;
		if (shoot > 1) {
			if (inTileX && inTileY) {
				if (rand() % 4 < 2) // Per canviar de moviment nomes la meitat de les ocasions
					move = rand() % 4;
			}
			switch (move) {
			case 0:
				MoveUp(map);
				break;
			case 1:
				MoveRight(map);
				break;
			case 2:
				MoveDown(map);
				break;
			case 3:
				MoveLeft(map);
				break;
			}
		}
		else {
			Stop();
			shooting = SHOOTING_TIME;
		}

		throwProjectil = false;

		UpdateMapTiles(map, x0, y0);
	}
	else {
		Stop();
		shooting--;
		if (shooting == 0)	throwProjectil = true;
	}
}

cBicho* cOctorok::ThrowProjectil(Tile* map) {
	if (throwProjectil) {
		cProjectil* projectil = new cProjectil();
		int x, y;
		GetPosition(&x, &y);
		projectil->SetPosition(x, y);
		projectil->SetState(GetState());
		projectil->SetWidthHeight(TILE_SIZE,TILE_SIZE);
		projectil->UpdateMapTiles(map, -1, -1);
		throwProjectil = false;
		return projectil;
	}
	else return nullptr;
}

bool cOctorok::Blockable() {
	return false;
}

bool cOctorok::ShieldBlocks(cBicho* bicho) {
	return false;
}

bool cOctorok::isFlyer() {
	return false;
}
