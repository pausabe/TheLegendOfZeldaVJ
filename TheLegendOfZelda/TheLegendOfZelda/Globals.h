#ifndef MY_HEADER_FILE_
#define MY_HEADER_FILE_

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <glut.h>
#include <utility>
#include <vector>
#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define TILE_SIZE		48
#define SCENE_Xo		0 //(2*TILE_SIZE)
#define SCENE_Yo		0 // TILE_SIZE
#define SCENE_WIDTH		16
#define SCENE_HEIGHT	11
#define BLOCK_SIZE		TILE_SIZE

// Informs the compiler that cBicho exists
class cBicho;

struct Tile {
	int tileId;
	bool isWall;
	std::vector<cBicho*> bichos = std::vector<cBicho*>(0);
};

class cRect
{

public:
	int left, top,
		right, bottom;
};


#endif

