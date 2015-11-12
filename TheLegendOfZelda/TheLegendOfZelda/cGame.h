#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"
#include "cOctorok.h"
#include "cDarknut.h"
#include "cTektike.h"
#include "cGanon.h"
#include "cStepladder.h"
#include "cKey.h"
#include <vector>

#define GAME_WIDTH	TILE_SIZE*16
#define GAME_HEIGHT TILE_SIZE*15

#define STATE_OVERWORLD_01 5
#define STATE_OVERWORLD_02 6
#define STATE_OVERWORLD_03 7
#define STATE_OVERWORLD_04 8

#define STATE_DUNGEON_01 15
#define STATE_DUNGEON_02 16
#define STATE_DUNGEON_03 17
#define STATE_DUNGEON_04 18
#define STATE_DUNGEON_05 19
#define STATE_DUNGEON_06 20
#define STATE_DUNGEON_07 21
#define STATE_DUNGEON_08 22

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

	int getStateScene();
	void setStateScene(int ss);

private:
	void printHearts();
	void createPanel();
	void setSceneState();
	int stateScene;
	int lastStateScene;
	int lastNumTexture;

	unsigned char keys[256];
	char selKey;
	int oneKey;

	cOctorok* c;
	cTektike* d;
	cScene Scene;
	cPlayer Player;
	cProjectil *Espasa;
	cData Data;
	std::vector<cBicho*> Enemies = std::vector<cBicho*>();
	int lifes;
	bool sKeyPressed = false;

	void DetectCollisions(std::vector<cBicho*> *bichos);
	void ProcessDynamicCollisions();
};