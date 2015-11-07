
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::SetPosition(int posx, int posy, Tile* map)
{
	x = posx;
	y = posy;
}
void cPlayer::SetTile(int tx, int ty, Tile *map)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	float sprite_size = 16;
    float texture_size = 512;

	int state = GetState();

	if (atacking != -1) {
		switch (state) {
		case STATE_ATACKDOWN:
			xo = 0.0f;
			yo = ((float)((sprite_size + 14) / texture_size)) * 2 + ((float)((sprite_size + 8) / texture_size));
			break;

		case STATE_ATACKUP:
			xo = (float)((sprite_size + 14) / texture_size) * 2;
			yo = ((float)((sprite_size + 14) / texture_size)) * 2 + ((float)((sprite_size + 8) / texture_size));
			break;

		case STATE_ATACKRIGHT:
			xo = (float)((sprite_size + 14) / texture_size) * 2 + ((float)((sprite_size + 8) / texture_size));
			yo = ((float)((sprite_size + 14) / texture_size)) * 3;
			break;

		case STATE_ATACKLEFT:
			xo = ((float)((sprite_size + 8) / texture_size));
			yo = ((float)((sprite_size + 14) / texture_size)) * 3;
			break;
		}
	}
	else {
		switch (state)
		{
		case STATE_LOOKLEFT:
			xo = (float)((sprite_size + 14) / texture_size);
			yo = 0.0f;
			break;

		case STATE_LOOKRIGHT:
			xo = (float)((sprite_size + 14) / texture_size) * 3;
			yo = 0.0f;
			break;

		case STATE_WALKLEFT:
			xo = (float)((sprite_size + 14) / texture_size);
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;

		case STATE_WALKRIGHT:
			xo = (float)((sprite_size + 14) / texture_size) * 3;
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;

		case STATE_LOOKUP:
			xo = (float)((sprite_size + 14) / texture_size) * 2;
			yo = 0.0f;
			break;

		case STATE_LOOKDOWN:
			xo = 0.0f;
			yo = 0.0f;
			break;

		case STATE_WALKUP:
			xo = (float)((sprite_size + 14) / texture_size) * 2;
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;

		case STATE_WALKDOWN:
			xo = 0.0f;
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;
		}
	}

	if(state == STATE_ATACKRIGHT || state == STATE_ATACKLEFT) xf = xo + (float)((sprite_size + 11.0f)/ texture_size);
	else xf = xo + (float)(sprite_size / texture_size);
	if (state == STATE_ATACKDOWN || state == STATE_ATACKUP) yf = yo + (float)((sprite_size + 11.0f) / texture_size);
	else yf = yo + (float)(sprite_size / texture_size);


	if (immune > 0 && immune % 4 > 2) glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
	DrawRect(tex_id,xo,yf,xf,yo);
	if (immune > 0 && immune % 4 > 2) glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void cPlayer::Stop()
{
	switch (state)
	{
	case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
	case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
	case STATE_WALKUP:		state = STATE_LOOKUP;		break;
	case STATE_WALKDOWN:	state = STATE_LOOKDOWN;		break;

	}
}

void cPlayer::Atack(Tile *map)
{
	if (atacking == -1) {
		atacking = ATACK_DURATION;

		throwProjectil = true;

		if (state == STATE_WALKDOWN || state == STATE_LOOKDOWN || state == STATE_ATACKDOWN) {
			state = STATE_ATACKDOWN;
			SetWidthHeight(TILE_SIZE, TILE_SIZE * 3 / 2);

			int x, y;
			x = GetPosX();
			y = GetPosY();
			SetPosition(x, (y - TILE_SIZE / 2), map);
		}
		else if (state == STATE_WALKUP || state == STATE_LOOKUP || state == STATE_ATACKUP) {
			state = STATE_ATACKUP;
			SetWidthHeight(TILE_SIZE, TILE_SIZE * 3 / 2);
		}
		else if (state == STATE_WALKRIGHT || state == STATE_LOOKRIGHT || state == STATE_ATACKRIGHT) {
			SetState(STATE_ATACKRIGHT);
			SetWidthHeight(TILE_SIZE * 3 / 2, TILE_SIZE);
		}
		else if (state == STATE_WALKLEFT || state == STATE_LOOKLEFT || state == STATE_ATACKLEFT) {
			state = STATE_ATACKLEFT;
			SetWidthHeight(TILE_SIZE * 3 / 2, TILE_SIZE);

			int x, y;
			x = GetPosX();
			y = GetPosY();
			SetPosition((x - TILE_SIZE / 2), y, map);
		}
	}

}

cBicho* cPlayer::ThrowProjectil(Tile* map) {
	if (throwProjectil) {
		cEspasa* projectil = new cEspasa();
		int x, y;
		GetPosition(&x, &y);
		projectil->SetPosition(x, y);
		projectil->SetState(GetState());
		projectil->SetWidthHeight(TILE_SIZE, TILE_SIZE);
		throwProjectil = false;
		return projectil;
	}
	else return nullptr;
}

void cPlayer::Logic(Tile* map) {	

	if (atacking > 0) {
		atacking--;
	} 
	else if (atacking == 0){
		SetWidthHeight(TILE_SIZE, TILE_SIZE);
		int x, y;
		switch (GetState()) {
		case STATE_ATACKDOWN: 
			x = GetPosX();
			y = GetPosY();
			SetPosition(x, (y + TILE_SIZE/2), map);
			SetState(STATE_LOOKDOWN);
			break;
		case STATE_ATACKUP:
			SetState(STATE_LOOKUP);
			break;
		case STATE_ATACKLEFT:
			x = GetPosX();
			y = GetPosY();
			SetPosition((x + TILE_SIZE/2), y, map);
			SetState(STATE_LOOKLEFT);
			break;
		case STATE_ATACKRIGHT:
			SetState(STATE_LOOKRIGHT);
			break;
		}
		atacking = -1;
	}


	if (jumping != -1) {
		SetWidthHeight(TILE_SIZE, TILE_SIZE);
		atacking = -1;
		int state = GetState();
		stepLength = JUMP_STEP;
		switch (jumping) {
		case LEFT:
			MoveLeft(map);
			break;
		case UP:
			MoveUp(map);
			break;
		case RIGHT:
			MoveRight(map);
			break;
		case DOWN:
			MoveDown(map);
			break;
		}
		jump -= stepLength;
		if (jump <= 0) {
			jumping = -1;
		}
		SetState(state);
	} else stepLength = STEP_LENGTH;


	if (immune > 0) {
		immune--;
	}
}

bool cPlayer::isJumping() {
	return jumping != -1;
}

void cPlayer::JumpBack(cRect* collider) {
	int diffX = x - collider->left;
	int diffY = y - collider->bottom;

	if (diffX > diffY && diffX < 0) jumping = LEFT;
	else if (diffX > diffY && diffX >= 0) jumping = RIGHT;
	else if (diffX <= diffY && diffY < 0) jumping = DOWN;
	else if (diffX <= diffY && diffY >= 0) jumping = UP;
	jump = JUMP_LENGTH;

	int state = GetState();
	if (state == STATE_ATACKDOWN || state == STATE_WALKDOWN) {
		atacking = -1;
		SetState(STATE_LOOKDOWN);
	}
	else if (state == STATE_ATACKUP || state == STATE_WALKUP) {
		atacking = -1;
		SetState(STATE_LOOKUP);
	}
	else if (state == STATE_ATACKLEFT || state == STATE_WALKLEFT) {
		atacking = -1;
		SetState(STATE_LOOKLEFT);
	}
	else if (state == STATE_ATACKRIGHT || state == STATE_WALKRIGHT) {
		atacking = -1;
		SetState(STATE_LOOKRIGHT);
	}
}

void cPlayer::Hit(cRect* collider) {
	JumpBack(collider);
	immune = IMMUNITY_DURATION;
	lifes--;
}

int cPlayer::getAtacking()
{
	return atacking;
}


bool cPlayer::isImmune() {
	return immune > 0;
}

bool cPlayer::Blockable() {
	return true;
}