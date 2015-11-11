#include "cBicho.h"

cBicho::cBicho(void)
{
	seq=0;
	delay=0;

	jumping = false;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}

int cBicho::GetPosX() {
	return x;
}
int cBicho::GetPosY() {
	return y;
}
void cBicho::SetTile(int tx,int ty)
{

	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
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

void cBicho::RemoveBichoFromVector(std::vector<cBicho*> *bichos, cBicho* bicho) {
	for (int i = 0; i < bichos->size(); i++) {
		if ((*bichos)[i] == bicho) {
			bichos->erase(bichos->begin() + i);
		}
	}
}

void cBicho::UpdateMapTiles(Tile *map, int x0, int y0) {
	
	// Remove the bicho from original tiles
	if (x0 >= 0 && y0 >= 0 && y0 < TILE_SIZE*11 && x0 < TILE_SIZE*16) {
		std::vector<cBicho*> *bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
		for (int i = 0; i < bichos->size(); i++) {
			if ((*bichos)[i] == this) {
				bichos->erase(bichos->begin() + i);
				break;
			}
		}

		if (x0 % TILE_SIZE != 0 && x0 + TILE_SIZE < TILE_SIZE * 16) {
			bichos = &map[((y0 / TILE_SIZE)*SCENE_WIDTH) + x0 / TILE_SIZE + 1].bichos;
			for (int i = 0; i < bichos->size(); i++) {
				if ((*bichos)[i] == this) {
					bichos->erase(bichos->begin() + i);
					break;
				}
			}
		}

		if (y0 % TILE_SIZE != 0 && y0 + TILE_SIZE < TILE_SIZE * 11) {
			bichos = &map[(((y0 / TILE_SIZE) + 1)*SCENE_WIDTH) + x0 / TILE_SIZE].bichos;
			for (int i = 0; i < bichos->size(); i++) {
				if ((*bichos)[i] == this) {
					bichos->erase(bichos->begin() + i);
					break;
				}
			}
		}

		if (x0 % TILE_SIZE != 0 && y0 % TILE_SIZE != 0 && y0 + TILE_SIZE < TILE_SIZE * 11 && x0 + TILE_SIZE < TILE_SIZE * 16) {
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
	if (x != -1 && y != -1 && x < TILE_SIZE*16 && y < TILE_SIZE*11) {
		map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
		if (x % TILE_SIZE != 0 && x + TILE_SIZE < TILE_SIZE*16)
			map[((y / TILE_SIZE)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
		if (y % TILE_SIZE != 0 && y + TILE_SIZE < TILE_SIZE*11)
			map[(((y / TILE_SIZE) + 1)*SCENE_WIDTH) + x / TILE_SIZE].bichos.push_back(this);
		if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0 && x + TILE_SIZE < TILE_SIZE*16 && y + TILE_SIZE < TILE_SIZE*11)
			map[(((y / TILE_SIZE) + 1)*SCENE_WIDTH) + x / TILE_SIZE + 1].bichos.push_back(this);
	}
}


void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == frameDelay)
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

void cBicho::Destroy(Tile* map) {
	toBeDestroyed = true;
	int x0;
	int y0;

	GetPosition(&x0, &y0);
	x = -1;
	y = -1;
	UpdateMapTiles(map, x0, y0);
}

int cBicho::GetLifes() {
	return lifes;
}

void cBicho::SetLifes(int l) {
	lifes = l;
}

void cBicho::Hit() {
	lifes--;
	if (lifes <= 0) toBeDestroyed = true;
}

