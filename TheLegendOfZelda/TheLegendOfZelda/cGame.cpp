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
	oneKey = true;
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
	Player.SetState(STATE_LOOKDOWN);
	Player.MoveLeft(Scene.GetMap());
	lifes = 3;

	//Enemies initialization
	res = Data.LoadImage(OVERWORLD_ENEMIES, "resources/overworld_enemies.png", GL_RGBA);
	if (!res) return false;

	cOctorok* c = new cOctorok();
	c->SetTile(4, 5);
	int x, y;
	c->UpdateMapTiles(Scene.GetMap(), -1,-1);
	c->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c->SetState(STATE_LOOKDOWN);
	
	cDarknut* d = new cDarknut();
	d->SetTile(6,5);
	d->UpdateMapTiles(Scene.GetMap(), -1, -1);
	d->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d->SetState(STATE_LOOKDOWN);

	Enemies.push_back(c);
	Enemies.push_back(d);
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

	if (!Player.isJumping()) {
		if ((keys['s'] || keys['S'])&& !sKeyPressed) {
			Player.Atack(Scene.GetMap());
			sKeyPressed = true;
		}
		if(Player.getAtacking() == -1){
			if (keys[GLUT_KEY_UP]) {
				Player.MoveUp(Scene.GetMap());
			}
			else if (keys[GLUT_KEY_DOWN])
			{
				Player.MoveDown(Scene.GetMap());
			}
			else if (keys[GLUT_KEY_LEFT])
			{
				Player.MoveLeft(Scene.GetMap());
			}
			else if (keys[GLUT_KEY_RIGHT])
			{
				Player.MoveRight(Scene.GetMap());
			}
			else Player.Stop();
		}
	}

	if (!keys['s']) sKeyPressed = false;

	//Game Logic
	Player.Logic(Scene.GetMap());

	if (Espasa == NULL) Espasa = dynamic_cast<cEspasa*>(Player.ThrowProjectil(Scene.GetMap()));
	else if (Espasa->ToBeDestroyed()) Espasa = NULL;
	else Espasa->Logic(Scene.GetMap());

	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->ToBeDestroyed()) {
			cBicho* aux = Enemies[i];
			aux->Destroy(Scene.GetMap());
			Enemies.erase(Enemies.begin() + i);
			//delete aux;
		}
		else {
			Enemies[i]->Logic(Scene.GetMap());
			cBicho* projectil = Enemies[i]->ThrowProjectil(Scene.GetMap());
			if (projectil != NULL) {
				Enemies.push_back(projectil);
				projectil->UpdateMapTiles(Scene.GetMap(), -1, -1);
			}
		}
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
	if (Espasa != NULL) Espasa->Draw(Data.GetID(LINK));
	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i]->Draw(Data.GetID(OVERWORLD_ENEMIES));
	}

	glutSwapBuffers();
}

bool cGame::ShieldBlocks(cBicho* bicho) {
	if (!dynamic_cast<cEnemy*>(bicho)->Blockable()) return false;
	int bichoState = bicho->GetState();
	int playerState = Player.GetState();
	if ((bichoState == STATE_WALKUP || bichoState == STATE_LOOKUP) && (playerState == STATE_WALKDOWN || playerState == STATE_LOOKDOWN)) return true;
	else if ((bichoState == STATE_WALKRIGHT || bichoState == STATE_LOOKRIGHT) && (playerState == STATE_WALKLEFT || playerState == STATE_LOOKLEFT)) return true;
	else if ((bichoState == STATE_WALKDOWN || bichoState == STATE_LOOKDOWN) && (playerState == STATE_WALKUP || playerState == STATE_LOOKUP)) return true;
	else if ((bichoState == STATE_WALKLEFT || bichoState == STATE_LOOKLEFT) && (playerState == STATE_WALKRIGHT || playerState == STATE_LOOKRIGHT)) return true;
	return false;
}

void cGame::DetectCollisions(std::vector<cBicho*> *bichos) {
	for (int i = 0; i < bichos->size(); i++) {
		cRect rt;
		(*bichos)[i]->GetArea(&rt);
		if (ShieldBlocks((*bichos)[i])) dynamic_cast<cTerrestre*>((*bichos)[i])->Destroy(Scene.GetMap());
		else if (Player.Collides(&rt) && !Player.isImmune()) {
			lifes--;
			Player.Hit(&rt);
		}
	}
}

void cGame::ProcessDynamicCollisions() {
	// Detect collisions with enemies
	int x, y;
	Player.GetPosition(&x, &y);
	std::vector<cBicho*> *bichos = &Scene.GetMap()[(y/TILE_SIZE)*SCENE_WIDTH + x / TILE_SIZE].bichos;

	if (x % TILE_SIZE != 0) {
		bichos = &Scene.GetMap()[(y / TILE_SIZE)*SCENE_WIDTH + x / TILE_SIZE + 1].bichos;
		DetectCollisions(bichos);
	}

	if (y % TILE_SIZE != 0) {
		bichos = &Scene.GetMap()[((y / TILE_SIZE) + 1)*SCENE_WIDTH + x / TILE_SIZE].bichos;
		DetectCollisions(bichos);
	}

	if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0) {
		bichos = &Scene.GetMap()[((y / TILE_SIZE) + 1)*SCENE_WIDTH + x / TILE_SIZE + 1].bichos;
		DetectCollisions(bichos);
	}

}