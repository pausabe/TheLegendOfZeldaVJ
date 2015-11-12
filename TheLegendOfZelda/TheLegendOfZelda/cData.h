#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG		6

//Image identifiers
#define OVERWORLD_TILES	0
#define LINK	1
#define OVERWORLD_ENEMIES	2
#define DUNGEON_TILES	3
#define BOSSES 4
#define TREASURES 5

class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	bool LoadImage(int img,char *filename,int type = GL_RGBA);


private:
	//int stateScene = STATE_OVERWORLD_01;
	cTexture texture[NUM_IMG];
};
