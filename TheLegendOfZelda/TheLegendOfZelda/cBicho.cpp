#include "cBicho.h"

cBicho::cBicho(void)
{
	seq=0;
	delay=0;

	jumping = false;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height, Tile* map)
{
	x = posx;
	y = posy;
	w = width;
	h = height;

	UpdateMapTiles(map, -1, -1);
}
void cBicho::SetPosition(int posx,int posy, Tile* map)
{
	int x0;
	int y0;

	GetPosition(&x0, &y0);

	x = posx;
	y = posy;

	UpdateMapTiles(map, x0, y0);
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx,int ty, Tile *map)
{
	int x0;
	int y0;

	GetPosition(&x0, &y0);

	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;

	UpdateMapTiles(map, x0, y0);
}
void cBicho::GetTile(int *tx,int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}
bool cBicho::Collides(cRect *rc)
{
	return ((x+w>=rc->left) && (x<=rc->right) && (y+h>=rc->bottom) && (y<=rc->top));
}
bool cBicho::CollidesMapWall(Tile *map,bool right)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if(right)	tile_x += width_tiles;
	
	for(j=0;j<height_tiles;j++)
	{
		if(map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ].tileId != 0)	return true;
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(Tile *map)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if( (x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if( (y % TILE_SIZE) == 0 )
		{
			if(map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ].tileId != 0)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ].tileId != 0)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}
void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	if (state == STATE_ATACKDOWN) {
		screen_y -= 32; 
	}
	else if (state == STATE_ATACKLEFT) screen_x -= 32;

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::MoveLeft(Tile *map)
{
	//SetWidthHeight(64, 64);
	int xaux;
	
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x -= STEP_LENGTH;

		if(CollidesMapWall(map,false))
		{
			x = xaux;
			state = STATE_LOOKLEFT;
		}
	}
	//Advance, no problem
	else
	{
		x -= STEP_LENGTH;
		if(state != STATE_WALKLEFT)
		{
			state = STATE_WALKLEFT;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::MoveRight(Tile *map)
{
	int xaux;

	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x += STEP_LENGTH;

		if(CollidesMapWall(map,true))
		{
			x = xaux;
			state = STATE_LOOKRIGHT;
		}
	}
	//Advance, no problem
	else
	{
		x += STEP_LENGTH;

		if(state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::MoveUp(Tile *map)
{
	int yaux;

	//Whats next tile?
	if ((y % TILE_SIZE) == 0)
	{
		yaux = y;
		y += STEP_LENGTH;

		if (CollidesMapWall(map, true))
		{
			y = yaux;
			state = STATE_LOOKUP;
		}
	}
	//Advance, no problem
	else
	{
		y += STEP_LENGTH;

		if (state != STATE_WALKUP)
		{
			state = STATE_WALKUP;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::MoveDown(Tile *map)
{
	int yaux;

	//Whats next tile?
	if ((y % TILE_SIZE) == 0)
	{
		yaux = y;
		y -= STEP_LENGTH;

		if (CollidesMapWall(map, true))
		{
			y = yaux;
			state = STATE_LOOKDOWN;
		}
	}
	//Advance, no problem
	else
	{
		y -= STEP_LENGTH;

		if (state != STATE_WALKDOWN)
		{
			state = STATE_WALKDOWN;
			seq = 0;
			delay = 0;
		}
	}
}

void cBicho::Stop()
{
	//SetWidthHeight(64, 64);
	switch(state)
	{
		case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
		case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
		case STATE_WALKUP:		state = STATE_LOOKUP;		break;
		case STATE_WALKDOWN:	state = STATE_LOOKDOWN;		break;
		case STATE_ATACKLEFT:	state = STATE_LOOKLEFT;		break;
		case STATE_ATACKRIGHT:	state = STATE_LOOKRIGHT;	break;
		case STATE_ATACKUP:		state = STATE_LOOKUP;		break;
		case STATE_ATACKDOWN:	state = STATE_LOOKDOWN;		break;

	}
}

void cBicho::UpdateMapTiles(Tile *map, int x0, int y0) {
	
	// Remove the bicho from original tiles
	if (x0 >= 0 && y0 >= 0) {
		std::vector<cBicho*> *bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
		for (int i = 0; i < bichos->size(); i++) {
			if ((*bichos)[i] == this) {
				bichos->erase(bichos->begin() + i);
				break;
			}
		}

		if (x0 % TILE_SIZE != 0) {
			bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE + 1].bichos;
			for (int i = 0; i < bichos->size(); i++) {
				if ((*bichos)[i] == this) {
					bichos->erase(bichos->begin() + i);
					break;
				}
			}
		}

		if (y0 % TILE_SIZE != 0) {
			bichos = &map[(((y0 / TILE_SIZE) + 1)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
			for (int i = 0; i < bichos->size(); i++) {
				if ((*bichos)[i] == this) {
					bichos->erase(bichos->begin() + i);
					break;
				}
			}
		}

		if (x0 % TILE_SIZE != 0 && y0 % TILE_SIZE != 0) {
			bichos = &map[(((y0 / TILE_SIZE) + 1)*SCENE_WIDTH) + x0 / TILE_SIZE + 1].bichos;
			for (int i = 0; i < bichos->size(); i++) {
				if ((*bichos)[i] == this) {
					bichos->erase(bichos->begin() + i);
					break;
		}
	}
		}
	}

	// Add the bicho to the map with the new tiles
	int x, y;
	GetPosition(&x, &y);
	map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
	if (x % TILE_SIZE != 0)
		map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
	if (y % TILE_SIZE != 0)
		map[(((y / TILE_SIZE)+1)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
	if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0)
		map[(((y / TILE_SIZE)+1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);

}
/*
void cBicho::Logic(Tile *map)
{
	int x0;
	int y0;

	GetPosition(&x0, &y0);

	// Move the bicho

	UpdateMapTiles(map, x0, y0);

}

*/

void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == FRAME_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cBicho::GetFrame()
{
	return seq;
}

int cBicho::GetState()
{
	return state;
}

void cBicho::SetState(int s)
{
	state = s;
}

bool cBicho::ToBeDestroyed() {
	return toBeDestroyed;
}