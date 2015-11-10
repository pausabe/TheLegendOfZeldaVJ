#pragma once

#include "Globals.h"

#include "cTexture.h"
#include "cScene.h"

#define FRAME_DELAY		4

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
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	int GetPosX();
	int GetPosY();
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(Tile *map,bool right);
	bool CollidesMapFloor(Tile *map);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	/*void MoveRight(Tile *map);
	void MoveLeft(Tile *map);
	void MoveUp(Tile *map);
	void MoveDown(Tile *map);*/
	//void Atack(Tile * map);
	//void Jump(int *map);
	//void Stop();

	void RemoveBichoFromVector(std::vector<cBicho*> *bichos, cBicho*);
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
	virtual bool Blockable() = 0;

	bool ToBeDestroyed();
	void Destroy(Tile* map);

	void SetLifes(int lifes);
	int GetLifes();

	void Hit(Tile* map);

protected:
	int x = -1, y = -1;
	int w, h;
	int state;
	int lastState;
	int seq, delay;
	bool throwProjectil = false;
	bool toBeDestroyed = false;
	int lifes;
	int frameDelay = FRAME_DELAY;

private:

	bool jumping;
};
