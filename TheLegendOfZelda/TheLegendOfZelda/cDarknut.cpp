#include "cDarknut.h"

cDarknut::cDarknut() {
	stepLength = 2;
}
cDarknut::~cDarknut() {}

void cDarknut::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	//hi ha 14 pixels en blanc que hem d'anar saltant
	switch (GetState())
	{
		//1
	case STATE_LOOKLEFT:	xo = (float)((sprite_size + 14) * 5 / texture_size);	yo = (float)((sprite_size + 14) * 6 / texture_size);;
		break;
		//4
	case STATE_LOOKRIGHT:	xo = (float)((sprite_size + 14) * 7 / texture_size);	yo = (float)((sprite_size + 14) * 6 / texture_size);;
		break;
		//1..3
	case STATE_WALKLEFT:	xo = (float)((sprite_size + 14) * 5 / texture_size);
		yo = (float)(sprite_size + 14) * 6 / texture_size + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;
		//4..6
	case STATE_WALKRIGHT:	xo = (float)((sprite_size + 14) * 7 / texture_size);
		yo = (float)(sprite_size + 14) * 6 / texture_size + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;

	case STATE_LOOKUP:		xo = (float)((sprite_size + 14) * 6 / texture_size);	yo = (float)((sprite_size + 14) * 6 / texture_size);;
		break;

	case STATE_LOOKDOWN:	xo = (float)((sprite_size + 14) * 4 / texture_size);	yo = (float)((sprite_size + 14) * 6 / texture_size);
		break;

	case STATE_WALKUP:		xo = (float)((sprite_size + 14) * 6 / texture_size);
		yo = (float)(sprite_size + 14) * 6 / texture_size + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;

	case STATE_WALKDOWN:	xo = (float)((sprite_size + 14) * 4 / texture_size);;
		yo = (float)(sprite_size + 14) * 6 / texture_size + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
		NextFrame(2);
		break;
	}


	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	DrawRect(tex_id, xo, yf, xf, yo);
}

void cDarknut::Logic(Tile *map) {
		int x0;
		int y0;

		GetPosition(&x0, &y0);

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

		UpdateMapTiles(map, x0, y0);
}

cBicho* cDarknut::ThrowProjectil(Tile* map) {
	return nullptr;
}

bool cDarknut::Blockable() {
	return false;
}

bool cDarknut::ShieldBlocks(cBicho* bicho) {
	if (!bicho->Blockable()) return false;
	int bichoState = bicho->GetState();
	int selfState = GetState();
	if ((bichoState == STATE_WALKUP || bichoState == STATE_LOOKUP) && (selfState == STATE_WALKDOWN || selfState == STATE_LOOKDOWN)) return true;
	else if ((bichoState == STATE_WALKRIGHT || bichoState == STATE_LOOKRIGHT) && (selfState == STATE_WALKLEFT || selfState == STATE_LOOKLEFT)) return true;
	else if ((bichoState == STATE_WALKDOWN || bichoState == STATE_LOOKDOWN) && (selfState == STATE_WALKUP || selfState == STATE_LOOKUP)) return true;
	else if ((bichoState == STATE_WALKLEFT || bichoState == STATE_LOOKLEFT) && (selfState == STATE_WALKRIGHT || selfState == STATE_LOOKRIGHT)) return true;
	return false;
}
