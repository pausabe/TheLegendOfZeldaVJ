#include "cTes.h"
#include <queue>

cTes::cTes() {
	time = TIME;
}
cTes::~cTes() {}

void cTes::Draw(int tex_id)
{
	float xo, yo, xf, yf;

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

	xo += (float)4*30/512;

	if (state == STATE_ATACKRIGHT || state == STATE_ATACKLEFT) xf = xo + (float)((sprite_size + 11.0f) / texture_size);
	else xf = xo + (float)(sprite_size / texture_size);
	if (state == STATE_ATACKDOWN || state == STATE_ATACKUP) yf = yo + (float)((sprite_size + 11.0f) / texture_size);
	else yf = yo + (float)(sprite_size / texture_size);


	if (immune > 0 && immune % 4 > 2) glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
	DrawRect(tex_id, xo, yf, xf, yo);
	if (immune > 0 && immune % 4 > 2) glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

int cTes::BFS(Tile* map) {
	int tx, ty;
	GetTile(&tx, &ty);

	std::vector<bool> visited(SCENE_WIDTH*SCENE_HEIGHT, false);
	std::queue< std::pair<int, int> > queue; // First: actual tile. Second: first move to go there
	if (tx >= 1 && !visited[ty*SCENE_WIDTH + tx - 1] && !map[ty*SCENE_WIDTH + tx - 1].isWall)
		queue.push(std::pair<int, int>(ty*SCENE_WIDTH + tx - 1, LEFT));
	if (ty < SCENE_HEIGHT - 1 && !visited[(ty + 1)*SCENE_WIDTH + tx] && !map[(ty + 1)*SCENE_WIDTH + tx].isWall)
		queue.push(std::pair<int, int>((ty + 1)*SCENE_WIDTH + tx, UP));
	if (tx < SCENE_WIDTH - 1 && !visited[ty*SCENE_WIDTH + tx + 1] && !map[ty*SCENE_WIDTH + tx + 1].isWall)
		queue.push(std::pair<int, int>(ty*SCENE_WIDTH + tx + 1, RIGHT));
	if (ty >= 1 && !visited[(ty - 1)*SCENE_WIDTH + tx] && !map[(ty - 1)*SCENE_WIDTH + tx].isWall)
		queue.push(std::pair<int, int>((ty - 1)*SCENE_WIDTH + tx, DOWN));

	int tile;
	bool trobat = false;
	while (!queue.empty() && !trobat)
	{
		tile = queue.front().first;

		if (map[tile].bichos.size() > 0 && map[tile].bichos[0]->GetPosX() != -1) trobat = true;
		else {
			int move = queue.front().second;
			visited[tile] = true;
			queue.pop();
			tx = tile % SCENE_WIDTH;
			ty = tile / SCENE_WIDTH;
			bool b = map[ty*SCENE_WIDTH + tx - 1].isWall; 
			if (tx >= 1 && !visited[ty*SCENE_WIDTH + tx - 1] && !map[ty*SCENE_WIDTH + tx - 1].isWall) 
				queue.push(std::pair<int, int>(ty*SCENE_WIDTH + tx - 1, move));
			if (ty < SCENE_HEIGHT - 1 && !visited[(ty + 1)*SCENE_WIDTH + tx] && !map[(ty + 1)*SCENE_WIDTH + tx].isWall) 
				queue.push(std::pair<int, int>((ty + 1)*SCENE_WIDTH + tx, move));
			if (tx < SCENE_WIDTH - 1 && !visited[ty*SCENE_WIDTH + tx + 1] && !map[ty*SCENE_WIDTH + tx + 1].isWall) 
				queue.push(std::pair<int, int>(ty*SCENE_WIDTH + tx + 1, move));
			if (ty >= 1 && !visited[(ty - 1)*SCENE_WIDTH + tx] && !map[(ty - 1)*SCENE_WIDTH + tx].isWall) 
				queue.push(std::pair<int, int>((ty - 1)*SCENE_WIDTH + tx, move));
		}
	}
	
	if (queue.empty()) return -1;
	else return queue.front().second;
}

void cTes::Logic(Tile* map) {
}

void cTes::Logic(Tile* map, std::vector<cBicho*> Enemies) {
	if (time > 0) {
		time--;
		if (atacking > 0) {
			atacking--;
		}
		else if (atacking == 0) {
			SetWidthHeight(TILE_SIZE, TILE_SIZE);
			int x, y;
			switch (GetState()) {
			case STATE_ATACKDOWN:
				x = GetPosX();
				y = GetPosY();
				SetPosition(x, (y + TILE_SIZE / 2));
				SetState(STATE_LOOKDOWN);
				break;
			case STATE_ATACKUP:
				SetState(STATE_LOOKUP);
				break;
			case STATE_ATACKLEFT:
				x = GetPosX();
				y = GetPosY();
				SetPosition((x + TILE_SIZE / 2), y);
				SetState(STATE_LOOKLEFT);
				break;
			case STATE_ATACKRIGHT:
				SetState(STATE_LOOKRIGHT);
				break;
			}
			atacking = -1;
		}
		else {
			int tx, ty;
			GetTile(&tx, &ty);
			cBicho* aux = NULL;
			if (map[ty*SCENE_WIDTH + tx].bichos.size() > 0) aux = map[ty*SCENE_WIDTH + tx].bichos[0];
			else if (map[ty*SCENE_WIDTH + tx + 1].bichos.size() > 0) aux = map[ty*SCENE_WIDTH + tx + 1].bichos[0];
			else if (map[ty*SCENE_WIDTH + tx - 1].bichos.size() > 0) aux = map[ty*SCENE_WIDTH + tx - 1].bichos[0];
			else if (map[(ty + 1)*SCENE_WIDTH + tx].bichos.size() > 0) aux = map[(ty + 1)*SCENE_WIDTH + tx].bichos[0];
			else if (map[(ty - 1)*SCENE_WIDTH + tx].bichos.size() > 0) aux = map[(ty - 1)*SCENE_WIDTH + tx].bichos[0];

			if (aux != NULL && aux->GetPosX() != -1 && (abs(x - aux->GetPosX()) < TILE_SIZE || abs(y - aux->GetPosY()) < TILE_SIZE)) {
				int diffX = x - aux->GetPosX();
				int diffY = y - aux->GetPosY();

				if (abs(diffX) > abs(diffY) && diffX < 0) SetState(STATE_LOOKRIGHT);
				else if (abs(diffX) > abs(diffY) && diffX >= 0) SetState(STATE_LOOKLEFT);
				else if (abs(diffX) <= abs(diffY) && diffY < 0) SetState(STATE_LOOKUP);
				else if (abs(diffX) <= abs(diffY) && diffY >= 0) SetState(STATE_LOOKDOWN);
				Atack(map);
			}
			else {
				if (Enemies.size() == 0) {
					move = -1;
					Stop();
				}
				else if (inTileX && inTileY) move = BFS(map);
				switch (move) {
				case -1:
					break;
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
			}
		}
	}
	else {
		toBeDestroyed = true;
	}
}