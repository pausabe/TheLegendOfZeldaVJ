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
#include "cDungeon.h"
#include "cTes.h"
#include <vector>

#define GAME_WIDTH	TILE_SIZE*16
#define GAME_HEIGHT TILE_SIZE*15

#define TES_COOLDOWN 20*50
#define GAME_OVER_ANIMATION 130

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

	void ClearEnemiesFromMap();
	void LoadOverworldEnemies();

private:
	void printObjectBox();
	void printHearts();
	void printMap();
	void createPanel();
	void setSceneState();
	int stateScene;
	int lastStateScene;
	int lastNumTexture;

	unsigned char keys[256];
	char selKey;
	int oneKey;
	int gameOver = GAME_OVER_ANIMATION;

	cScene Scene;
	cPlayer Player;
	cTes* Tes;
	cProjectil *Espasa;
	cData Data;
	cDungeon Dungeon;
	std::vector<cBicho*>* OverworldEnemies[4];// = std::vector<cBicho*>();
	std::vector<cBicho*> Enemies = std::vector<cBicho*>();
	std::vector<cObject*> Objects = std::vector<cObject*>();

	bool sKeyPressed = false;
	bool aKeyPressed = false;
	bool wallUnderStepladder = false;

	void DetectCollisions(std::vector<cBicho*> *bichos);
	void ProcessDynamicCollisions();

	void PlaceStepladder();
	int tesCooldown = 0;
};