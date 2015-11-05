#pragma once

#include "cTexture.h"
#include "Globals.h"
#include "cScene.h"

#define FRAME_DELAY		4
#define STEP_LENGTH		4

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_WALKLEFT		2
#define STATE_WALKRIGHT		3
#define STATE_LOOKUP		4
#define STATE_LOOKDOWN		5
#define STATE_WALKUP		6
#define STATE_WALKDOWN		7
#define STATE_ATACKLEFT		8
#define STATE_ATACKRIGHT	9
#define STATE_ATACKUP		10
#define STATE_ATACKDOWN		11


class cRect
{

public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h, Tile* map);
	~cBicho(void);

	void SetPosition(int x,int y, Tile* map);
	void GetPosition(int *x,int *y);
	void SetTile(int tx,int ty, Tile* map);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(Tile *map,bool right);
	bool CollidesMapFloor(Tile *map);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(Tile *map);
	void MoveLeft(Tile *map);
	void MoveUp(Tile *map);
	void MoveDown(Tile *màp);
	//void Atack(Tile * map);
	//void Jump(int *map);
	void Stop();

	// Updates the map tiles with the bicho position
	// tileX0 or tileY0 = -1 indicates that the bicho has just been created
	void UpdateMapTiles(Tile *map, int tileX0, int tileY0);

	virtual void Logic(Tile *map) = 0;

	int  GetState();
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();
	
	virtual void Draw(int tex_id) = 0;
	virtual cBicho* ThrowProjectil(Tile* map) = 0;
	
	bool ToBeDestroyed();

protected:
	int x = -1, y = -1;
	int w, h;
	int state;
	int lastState;
	int seq, delay;
	bool throwProjectil = false;
	bool toBeDestroyed = false;

private:

	bool jumping;
	int jump_alfa;
	int jump_y;

};
