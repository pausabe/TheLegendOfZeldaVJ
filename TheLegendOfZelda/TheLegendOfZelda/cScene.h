#pragma once

#include "cTexture.h"

#define SCENE_Xo		0 //(2*TILE_SIZE)
#define SCENE_Yo		0 // TILE_SIZE
#define SCENE_WIDTH		16
#define SCENE_HEIGHT	11

#define FILENAME		"level"
#define FILENAME_EXT	".txt"

#define TILE_SIZE		64
#define BLOCK_SIZE		TILE_SIZE

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(int level);
	void Draw(int tex_id);
	std::pair<int, bool> *GetMap();

private:

	// Returns wether the specified tile is a wall or not
	bool isAWall(int tile);

	// First: n� of tile, Second: if the tile is a wall or not
	std::pair<int,bool> map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
};
