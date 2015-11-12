#pragma once

#include "cTexture.h"
#include "cDungeon.h"


#define FILENAME			"levels/level"
#define DUNGEON_FILENAME	"levels/dungeon"
#define FILENAME_EXT		".txt"


class cScene
{

public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadOverworldLevel(int level);
	bool LoadDungeonLevel(int level, cDungeon Dungeon);
	void Draw(int tex_id);
	Tile *GetMap();

private:

	// Returns wether the specified tile is a wall or not
	bool isAWall(int tile);
	bool isADungeonWall(int tile);

	// Array of tiles
	Tile map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list

};
