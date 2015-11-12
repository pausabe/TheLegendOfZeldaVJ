#pragma once

#include "Globals.h"

class cObject
{
protected:
	int x, y;
	int w = TILE_SIZE;
	int h = TILE_SIZE;

public:
	cObject();
	~cObject();

	void DrawRect(int tex_id, float xo, float yo, float xf, float yf);

	void SetPosition(int x, int y);
	void GetPosition(int *x, int *y);
	void GetArea(cRect *rc);

	virtual void Draw(int tex_id) = 0;
	virtual int GetKeyId() = 0;

};
