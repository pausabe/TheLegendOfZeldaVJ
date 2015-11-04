#pragma once

#include "cTexture.h"
#include "Globals.h"

#define FRAME_DELAY			10
#define STEP_LENGTH			4

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
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(std::pair<int, bool> *map,bool right);
	bool CollidesMapFloor(std::pair<int, bool> *map);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(std::pair<int, bool> *map);
	void MoveLeft(std::pair<int, bool> *map);
	void MoveUp(std::pair<int, bool> *map);
	void MoveDown(std::pair<int, bool> *m�p);
	void Atack(std::pair<int, bool> *m�p);
	void Stop();
	void Logic(std::pair<int, bool> *map);

	int  GetState();
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();
	
protected:
	int x, y;
	int w, h;
	int state;
	int lastState;
	int seq, delay;

private:

	bool jumping;
	int jump_alfa;
	int jump_y;

};
