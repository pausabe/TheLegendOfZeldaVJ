#include "cTerrestre.h"
#include "cScene.h"

cTerrestre::cTerrestre() {}
cTerrestre::~cTerrestre() {}

bool cTerrestre::CollidesMapTile(std::pair<int, bool>* map, int direction) {
	int tile_x = x / TILE_SIZE;
	int tile_y = y / TILE_SIZE;
	int width_tiles = w / TILE_SIZE;
	int height_tiles = h / TILE_SIZE;

	int walls[] = {1};

	if (direction == 0) {				// LEFT
		if (y % TILE_SIZE > TILE_SIZE/2) height_tiles++;
		for (int i = 0; i < height_tiles; ++i) {
			if (map[tile_x + ((tile_y + i)*SCENE_WIDTH)].second)	return true;
		}
	} else if (direction == 1) {		// TOP
		if (x % TILE_SIZE != 0) width_tiles++;
		for (int i = 0; i < width_tiles; ++i) {
			if (map[tile_x + i + ((tile_y + 1)*SCENE_WIDTH)].second)	return true;
		}
	} else if (direction == 2) {		// RIGHT
		if (y % TILE_SIZE > TILE_SIZE / 2) height_tiles++;
		for (int i = 0; i < height_tiles; ++i) {
			if (map[tile_x + width_tiles + ((tile_y + i)*SCENE_WIDTH)].second)	return true;
		}
	} else if(direction == 3) {			// BOTTOM
		if (x % TILE_SIZE != 0) width_tiles++;
		for (int i = 0; i < width_tiles; ++i) {
			if (map[tile_x + i + ((tile_y)*SCENE_WIDTH)].second)	return true;
		}
	}

	return false;
}

void cTerrestre::MoveLeft(std::pair<int, bool> *map)
{
	int xaux;

	//Whats next tile?
	if ((x % TILE_SIZE) == 0)
	{
		xaux = x;
		x -= STEP_LENGTH;

		if (CollidesMapTile(map, 0))
		{
			x = xaux;
			state = STATE_LOOKLEFT;
		}
	}
	//Advance, no problem
	else
	{
		x -= STEP_LENGTH;
		if (state != STATE_WALKLEFT)
		{
			state = STATE_WALKLEFT;
			//seq = 0;
			//delay = 0;
		}
	}
}
void cTerrestre::MoveRight(std::pair<int, bool> *map)
{
	int xaux;

	//Whats next tile?
	if ((x % TILE_SIZE) == 0)
	{
		xaux = x;
		x += STEP_LENGTH;

		if (CollidesMapTile(map, 2))
		{
			x = xaux;
			state = STATE_LOOKRIGHT;
		}
	}
	//Advance, no problem
	else
	{
		x += STEP_LENGTH;

		if (state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			//seq = 0;
			//delay = 0;
		}
	}
}
void cTerrestre::MoveUp(std::pair<int, bool> *map)
{
	int yaux;

	//Whats next tile?
	if ((y % TILE_SIZE) == TILE_SIZE / 2)
	//if ((y % TILE_SIZE) == 0)
	{
		yaux = y;
		y += STEP_LENGTH;

		if (CollidesMapTile(map, 1))
		{
			y = yaux;
			state = STATE_LOOKRIGHT;
		}
	}
	//Advance, no problem
	else
	{
		y += STEP_LENGTH;

		if (state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			//seq = 0;
			//delay = 0;
		}
	}
}
void cTerrestre::MoveDown(std::pair<int, bool> *map)
{
	int yaux;

	//Whats next tile?
	if ((y % TILE_SIZE) == 0)
	{
		yaux = y;
		y -= STEP_LENGTH;

		if (CollidesMapTile(map, 3))
		{
			y = yaux;
			state = STATE_LOOKRIGHT;
		}
	}
	//Advance, no problem
	else
	{
		y -= STEP_LENGTH;

		if (state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			//seq = 0;
			//delay = 0;
		}
	}
}