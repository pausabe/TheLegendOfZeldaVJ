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
	oneKey = false;
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
	Player.SetTile(8, 5);
	Player.SetWidthHeight(TILE_SIZE, TILE_SIZE);
	Player.SetState(STATE_LOOKRIGHT);

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
	selKey = key;
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

	Player.SetWidthHeight(64, 64);

	if (selKey == 's') {
		if(Player.GetAtackFrame() == 2) selKey = ' ';
		Player.Atack(Scene.GetMap());
	}
	else if (keys[GLUT_KEY_UP] && selKey != 's') {
		Player.MoveUp(Scene.GetMap()); 
	}
	else if (keys[GLUT_KEY_DOWN] && selKey != 's')
	{
		Player.MoveDown(Scene.GetMap());
	}
	else if (keys[GLUT_KEY_LEFT] && selKey != 's')
	{
		Player.MoveLeft(Scene.GetMap());
	}
	else if (keys[GLUT_KEY_RIGHT] && selKey != 's')
	{
		Player.MoveRight(Scene.GetMap());
	}
	
	else Player.Stop();


	//Game Logic
	Player.Logic(Scene.GetMap());

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(OVERWORLD_TILES));

	Player.Draw(Data.GetID(LINK));

	glutSwapBuffers();
}
