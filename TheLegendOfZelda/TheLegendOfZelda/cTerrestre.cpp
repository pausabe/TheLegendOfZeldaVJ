#include "cTerrestre.h"
#include "cScene.h"

cTerrestre::cTerrestre() {
}
cTerrestre::~cTerrestre() {}

bool cTerrestre::CollidesMapTile(Tile* map, int direction) {
	int tile_x = x / TILE_SIZE;
	int tile_y = y / TILE_SIZE;
	int width_tiles = w / TILE_SIZE;
	int height_tiles = h / TILE_SIZE;

	int walls[] = {1};

	if (direction == LEFT) {				// LEFT
		if (y % TILE_SIZE > TILE_SIZE/2) height_tiles++;
		for (int i = 0; i < height_tiles; ++i) {
			if (map[tile_x + ((tile_y + i)*SCENE_WIDTH)].isWall)	return true;
		}
	} else if (direction == UP) {		// TOP
		if (x % TILE_SIZE != 0) width_tiles++;
		for (int i = 0; i < width_tiles; ++i) {
			if (map[tile_x + i + ((tile_y + 1)*SCENE_WIDTH)].isWall)	return true;
		}
	} else if (direction == RIGHT) {		// RIGHT
		if (y % TILE_SIZE > TILE_SIZE / 2) height_tiles++;
		for (int i = 0; i < height_tiles; ++i) {
			if (map[tile_x + width_tiles + ((tile_y + i)*SCENE_WIDTH)].isWall)	return true;
		}
	} else if(direction == DOWN) {			// BOTTOM
		if (x % TILE_SIZE != 0) width_tiles++;
		for (int i = 0; i < width_tiles; ++i) {
			if (map[tile_x + i + ((tile_y)*SCENE_WIDTH)].isWall)	return true;
		}
	}

	return false;
}

void cTerrestre::MoveLeft(Tile *map)
{
	int xaux;
	//SetWidthHeight(64, 64);
	//Whats next tile?
	if (((x-stepLength) % TILE_SIZE) >= TILE_SIZE - stepLength)
	{
		xaux = x;
		x -= stepLength;

		if (CollidesMapTile(map, LEFT)) {
			if (xaux % TILE_SIZE == 0) x = xaux;
			else x = xaux - TILE_SIZE + (xaux % (TILE_SIZE));
			state = STATE_LOOKLEFT;
			collision = true;
		} else collision = false;

	}
	//Advance, no problem
	else
	{
		x -= stepLength;
		if (state != STATE_WALKLEFT)
		{
			state = STATE_WALKLEFT;
			seq = 0;
			delay = 0;
		}
		collision = false;
	}
	if ((x % TILE_SIZE) == 0) inTileX = true;
	else inTileX = false;
}
void cTerrestre::MoveRight(Tile *map)
{
	int xaux;
	//SetWidthHeight(64, 64);
	//Whats next tile?
	if (((x + stepLength) % TILE_SIZE) <= stepLength)
	{
		xaux = x;
		x += stepLength;

		if (CollidesMapTile(map, RIGHT))
		{
			if (xaux % TILE_SIZE == 0)	x = xaux;
			else x = xaux + TILE_SIZE -(xaux%(TILE_SIZE));
			state = STATE_LOOKRIGHT;			
			collision = true;
		} else collision = false;
	}
	//Advance, no problem
	else
	{
		x += stepLength;

		if (state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			seq = 0;
			delay = 0;
		}
		collision = false;
	}
	if ((x % TILE_SIZE) == 0) inTileX = true;
	else inTileX = false;
}
void cTerrestre::MoveUp(Tile *map)
{
	int yaux;
	//SetWidthHeight(64, 64);
	//Whats next tile?
	if (((y + stepLength) % TILE_SIZE) >= TILE_SIZE / 2 + stepLength)
	{
		yaux = y;
		y += stepLength;

		if (CollidesMapTile(map, UP))
		{
			if (yaux % TILE_SIZE == TILE_SIZE / 2) y = yaux;
			else y = yaux + TILE_SIZE / 2 - (yaux % (TILE_SIZE));
			state = STATE_LOOKUP;
			collision = true;
		} else collision = false;

	}
	//Advance, no problem
	else
	{
		y += stepLength;

		if (state != STATE_WALKUP)
		{
			state = STATE_WALKUP;
			seq = 0;
			delay = 0;
		}
		collision = false;
	}
	if ((y % TILE_SIZE) == 0 || (y % TILE_SIZE) == TILE_SIZE / 2) inTileY = true;
	else inTileY = false;
}

void cTerrestre::MoveDown(Tile *map)
{
	int yaux;

	//Whats next tile?
	if (((y - stepLength) % TILE_SIZE) >= TILE_SIZE - stepLength)
	{
		yaux = y;
		y -= stepLength;

		if (CollidesMapTile(map, DOWN))
		{
			if (yaux % TILE_SIZE == 0) y = yaux;
			else y = yaux - (TILE_SIZE + (yaux % (TILE_SIZE)));
			state = STATE_LOOKDOWN;
			collision = true;
		} else collision = false;

	}
	//Advance, no problem
	else
	{
		y -= stepLength;

		if (state != STATE_WALKDOWN)
		{
			state = STATE_WALKDOWN;
			seq = 0;
			delay = 0;
		}
		collision = false;
	}
	if ((y % TILE_SIZE) == 0) inTileY = true;
	else inTileY = false;
}