#ifndef MY_HEADER_FILE_
#define MY_HEADER_FILE_

#include <windows.h>
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

// Informs the compiler that cBicho exists
class cBicho;

struct Tile {
	int tileId;
	bool isWall;
	std::vector<cBicho*> bichos = std::vector<cBicho*>(0);
};

#endif

