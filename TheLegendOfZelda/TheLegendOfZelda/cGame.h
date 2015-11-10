#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"
#include "cOctorok.h"
#include "cDarknut.h"
#include "cTektike.h"
#include "cGanon.h"
#include <vector>

#define GAME_WIDTH	TILE_SIZE*16
#define GAME_HEIGHT TILE_SIZE*13

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

private:
	unsigned char keys[256];
	char selKey;
	int oneKey;
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