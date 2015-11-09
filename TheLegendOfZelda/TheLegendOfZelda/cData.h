#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG		4

//Image identifiers
#define OVERWORLD_TILES	0
#define LINK	1
#define OVERWORLD_ENEMIES	2
#define DUNGEON_TILES	3

class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	bool LoadImage(int img,char *filename,int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};
