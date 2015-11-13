
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

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

void cPlayer::DetectAtackCollisions(cRect swordArea, std::vector<cBicho*> *bichos) {
	for (int i = 0; i < bichos->size(); i++) {
		cRect rt;
		(*bichos)[i]->GetArea(&rt);
		if (!(*bichos)[i]->ShieldBlocks(this) && (*bichos)[i]->Collides(&swordArea)) {
			int w, h;
			(*bichos)[i]->GetWidthHeight(&w, &h);
			if (w == TILE_SIZE*2) dynamic_cast<cGanon*>((*bichos)[i])->Hit();
			else ((*bichos)[i])->Hit();
		}
	}
}

void cPlayer::Atack(Tile* map)
{
	if (atacking == -1) {
		atacking = ATACK_DURATION;

		if (GetLifes() == 6) throwProjectil = true;

		cRect swordArea;

		if (state == STATE_WALKDOWN || state == STATE_LOOKDOWN || state == STATE_ATACKDOWN) {
			state = STATE_ATACKDOWN;
			SetWidthHeight(TILE_SIZE, TILE_SIZE * 3 / 2);

			int x, y;
			x = GetPosX();
			y = GetPosY();
			y = y - TILE_SIZE / 2;
			SetPosition(x, y);

			swordArea.bottom = y;
			swordArea.left = x + TILE_SIZE*(float)7/16;
			swordArea.top = swordArea.bottom + TILE_SIZE*(float) 11 / 16;
			swordArea.right = swordArea.left + TILE_SIZE*(float) 3 / 16;

			std::vector<cBicho*> *bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);
			bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
			DetectAtackCollisions(swordArea, bichos);

		}
		else if (state == STATE_WALKUP || state == STATE_LOOKUP || state == STATE_ATACKUP) {
			state = STATE_ATACKUP;
			SetWidthHeight(TILE_SIZE, TILE_SIZE * 3 / 2);

			swordArea.top = y + TILE_SIZE*3/2;
			swordArea.left = x + TILE_SIZE*(float)7 / 16;
			swordArea.bottom = swordArea.top - TILE_SIZE*(float)11 / 16;
			swordArea.right = swordArea.left + TILE_SIZE*(float)3 / 16;

			std::vector<cBicho*> *bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);
			
			bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);
		
			bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
			DetectAtackCollisions(swordArea, bichos);
			
			bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
			DetectAtackCollisions(swordArea, bichos);
		}
		else if (state == STATE_WALKRIGHT || state == STATE_LOOKRIGHT || state == STATE_ATACKRIGHT) {
			SetState(STATE_ATACKRIGHT);
			SetWidthHeight(TILE_SIZE * 3 / 2, TILE_SIZE);

			swordArea.bottom = y + TILE_SIZE*(float)5 / 16;
			swordArea.right = x + TILE_SIZE*3/2;
			swordArea.top = swordArea.bottom + TILE_SIZE*(float)3 / 16;
			swordArea.left = swordArea.right - TILE_SIZE*(float)11 / 16;

			std::vector<cBicho*> *bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);

			bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);

			bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
			DetectAtackCollisions(swordArea, bichos);

			bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos;
			DetectAtackCollisions(swordArea, bichos);
		}
		else if (state == STATE_WALKLEFT || state == STATE_LOOKLEFT || state == STATE_ATACKLEFT) {
			state = STATE_ATACKLEFT;
			SetWidthHeight(TILE_SIZE * 3 / 2, TILE_SIZE);

			int x, y;
			x = GetPosX();
			y = GetPosY();
			x = x - TILE_SIZE / 2;
			SetPosition(x, y);

			swordArea.bottom = y + TILE_SIZE*(float)5/16;
			swordArea.left = x;
			swordArea.top = swordArea.bottom + TILE_SIZE*(float)3 / 16;
			swordArea.right = swordArea.left + TILE_SIZE*(float)11 / 16;


			std::vector<cBicho*> *bichos = &map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);
			bichos = &map[((y / TILE_SIZE + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos;
			DetectAtackCollisions(swordArea, bichos);

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
}

void cPlayer::Logic(Tile* map, cDungeon Dungeon) {

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
			SetPosition(x, (y + TILE_SIZE/2));
			SetState(STATE_LOOKDOWN);
			break;
		case STATE_ATACKUP:
			SetState(STATE_LOOKUP);
			break;
		case STATE_ATACKLEFT:
			x = GetPosX();
			y = GetPosY();
			SetPosition((x + TILE_SIZE/2), y);
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

bool cPlayer::HoldsKey(int key) {
	bool key1 = false;
	bool key2 = false;
	for (int i = 0; i < HoldingObjects.size(); i++) {
		if (HoldingObjects[i]->GetKeyId() == 1) key1 = true;
		if (HoldingObjects[i]->GetKeyId() == 2) key2 = true;
	}
	if (key == 1) return key1;
	else return key2;
}

bool cPlayer::HoldsStepladder() {
	for (int i = 0; i < HoldingObjects.size(); i++) {
		if (HoldingObjects[i]->GetKeyId() == -1) return true;
	}
	return false;
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

	if (lifes > 0) {
		PlaySound(TEXT("sounds/LOZ_Hurt.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else {
		PlaySound(TEXT("sounds/LOZ_Die.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
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

bool cPlayer::ShieldBlocks(cBicho* bicho) {
	if (!bicho->Blockable()) return false;
	int bichoState = bicho->GetState();
	int selfState = GetState();
	if ((bichoState == STATE_WALKUP || bichoState == STATE_LOOKUP) && (selfState == STATE_WALKDOWN || selfState == STATE_LOOKDOWN)) return true;
	else if ((bichoState == STATE_WALKRIGHT || bichoState == STATE_LOOKRIGHT) && (selfState == STATE_WALKLEFT || selfState == STATE_LOOKLEFT)) return true;
	else if ((bichoState == STATE_WALKDOWN || bichoState == STATE_LOOKDOWN) && (selfState == STATE_WALKUP || selfState == STATE_LOOKUP)) return true;
	else if ((bichoState == STATE_WALKLEFT || bichoState == STATE_LOOKLEFT) && (selfState == STATE_WALKRIGHT || selfState == STATE_LOOKRIGHT)) return true;
	return false;
}

void cPlayer::PickObject(cObject* o) {
	HoldingObjects.push_back(o);
}

cStepladder* cPlayer::DropStepladder() {
	for (int i = 0; i < HoldingObjects.size(); i++) {
		if (HoldingObjects[i]->GetKeyId() == -1) {
			cStepladder* s = dynamic_cast<cStepladder*>(HoldingObjects[i]);
			HoldingObjects.erase(HoldingObjects.begin() + i);
			return s;
		}
	}
}

bool cPlayer::isFlyer() {
	return false;
}