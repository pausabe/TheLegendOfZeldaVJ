#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(OVERWORLD_TILES, "resources/overworld_tiles.png", GL_RGB);
	if (!res) return false;
	res = Scene.LoadLevel(1);
	if (!res) return false;

	//Player initialization
	res = Data.LoadImage(LINK, "resources/link.png", GL_RGBA);
	if (!res) return false;
	Player.SetTile(8, 5, Scene.GetMap());
	Player.SetWidthHeight(TILE_SIZE, TILE_SIZE);
	Player.SetState(STATE_LOOKRIGHT);

	lifes = 3;

	//Enemies initialization
	res = Data.LoadImage(OVERWORLD_ENEMIES, "resources/overworld_enemies.png", GL_RGBA);
	if (!res) return false;

	cOctorok* c = new cOctorok();
	c->SetTile(4, 5, Scene.GetMap());
	//c->SetPosition(8*TILE_SIZE, 5*TILE_SIZE+16, Scene.GetMap());
	c->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c->SetState(STATE_LOOKDOWN);
	
	Enemies.push_back(c);
	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res = true;

	//Process Input
	if (keys[27])	res = false;

	if (!Player.isJumping()) {
		if (keys[GLUT_KEY_UP])			Player.MoveUp(Scene.GetMap());
		else if (keys[GLUT_KEY_DOWN])		Player.MoveDown(Scene.GetMap());
		else if (keys[GLUT_KEY_LEFT])		Player.MoveLeft(Scene.GetMap());
		else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
		else Player.Stop();
	}

	//Game Logic
	Player.Logic(Scene.GetMap());

	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i]->Logic(Scene.GetMap());
	}

	// Detect player-enemies collisions
	ProcessDynamicCollisions();

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(OVERWORLD_TILES));

	Player.Draw(Data.GetID(LINK));

	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i]->Draw(Data.GetID(OVERWORLD_ENEMIES));
	}

	glutSwapBuffers();
}

void cGame::ProcessDynamicCollisions() {
	// Detect collisions with enemies
	int x, y;
	Player.GetPosition(&x, &y);
	std::vector<cBicho*> *bichos = &Scene.GetMap()[(y/TILE_SIZE)*SCENE_WIDTH + x / TILE_SIZE].bichos;
	for (int i = 0; i < bichos->size(); i++) {
		cRect rt;
		(*bichos)[i]->GetArea(&rt);
		if (Player.Collides(&rt)) {
			lifes--;
			Player.JumpBack(&rt);
		}
	}

	if (x % TILE_SIZE != 0) {
		bichos = &Scene.GetMap()[(y / TILE_SIZE)*SCENE_WIDTH + x / TILE_SIZE + 1].bichos;
		for (int i = 0; i < bichos->size(); i++) {
			cRect rt;
			(*bichos)[i]->GetArea(&rt);
			if (Player.Collides(&rt)) {
				lifes--;
				Player.JumpBack(&rt);
			}
		}
	}

	if (y % TILE_SIZE != 0) {
		bichos = &Scene.GetMap()[((y / TILE_SIZE) + 1)*SCENE_WIDTH + x / TILE_SIZE].bichos;
		for (int i = 0; i < bichos->size(); i++) {
			cRect rt;
			(*bichos)[i]->GetArea(&rt);
			if (Player.Collides(&rt)) {
				lifes--;
				Player.JumpBack(&rt);
			}
		}
	}

	if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0) {
		bichos = &Scene.GetMap()[((y / TILE_SIZE) + 1)*SCENE_WIDTH + x / TILE_SIZE + 1].bichos;
		for (int i = 0; i < bichos->size(); i++) {
			cRect rt;
			(*bichos)[i]->GetArea(&rt);
			if (Player.Collides(&rt)) {
				lifes--;
				Player.JumpBack(&rt);
			}
		}
	}

}