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

	/*if (!Buffer.loadFromFile("sound.wav"))
	{
		res = false;
	}*/
	//sf::Sound Sound;
	//Sound.setBuffer(Buffer);
	//Sound.play();

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
	res = Data.LoadImage(DUNGEON_TILES, "resources/dungeon_tiles.png", GL_RGBA);
	if (!res) return false;
	res = Data.LoadImage(OVERWORLD_ENEMIES, "resources/overworld_enemies.png", GL_RGBA);
	if (!res) return false;
	res = Data.LoadImage(LINK, "resources/link.png", GL_RGBA);
	if (!res) return false;
	res = Data.LoadImage(BOSSES, "resources/bosses.png", GL_RGBA);
	if (!res) return false;



	stateScene = STATE_OVERWORLD_01;
	lastStateScene = STATE_OVERWORLD_01;
	lastNumTexture = OVERWORLD_TILES;

	Scene.LoadOverworldLevel(1);


	//Player initialization
	Player.SetTile(8, 5);
	Player.SetWidthHeight(TILE_SIZE, TILE_SIZE);
	Player.SetState(STATE_LOOKDOWN);
	Player.MoveLeft(Scene.GetMap());
	Player.SetLifes(6);

	//Enemies initialization

	/*cGanon* g = new cGanon();
	g->SetTile(4, 5);
	g->UpdateMapTiles(Scene.GetMap(), -1, -1);
	g->SetWidthHeight(TILE_SIZE*2, TILE_SIZE*2);
	g->SetState(STATE_INVISIBLE);
	g->SetLifes(6);*/

	/*
	cOctorok* c = new cOctorok();
	c->SetTile(4, 5);
	int x, y;
	c->UpdateMapTiles(Scene.GetMap(), -1,-1);
	c->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	c->SetState(STATE_LOOKDOWN);
	
	cTektike* d = new cTektike();
	d->SetTile(6,5);
	d->UpdateMapTiles(Scene.GetMap(), -1, -1);
	d->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d->SetState(STATE_RESTING);
	*/
	//Enemies.push_back(g);
	//Enemies.push_back(d);

	cStepladder* s = new cStepladder();
	s->SetPosition(0,0);
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



void cGame::createPanel()
{

	/*int screen_x, screen_y;

	screen_x = SCENE_Xo;
	screen_y = SCENE_Yo + TILE_SIZE*11;

	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	xo = (float)((sprite_size + 14) * 11 / texture_size);
	yo = (float)0;
	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	int w = 16;
	glEnable(GL_TEXTURE_2D);

	
	glBindTexture(GL_TEXTURE_2D, OVERWORLD_ENEMIES);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y + w);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + w);
	glEnd();*/

	printHearts();

	glDisable(GL_TEXTURE_2D);
}

void cGame::printHearts()
{
	int screen_x, screen_y;

	screen_x = SCENE_Xo;
	screen_y = SCENE_Yo + TILE_SIZE * 11;

	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	xo = (float)((sprite_size + 14) * 11 / texture_size);
	yo = (float)0;
	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	int w = 16;
	glEnable(GL_TEXTURE_2D);


	int playerLives = Player.GetLifes();


	glBindTexture(GL_TEXTURE_2D, TREASURES);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y + w);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + w);
	glEnd();

}



void cGame::setSceneState() {
	int tile_x, tile_y;
	Player.GetTile(&tile_x, &tile_y);

	int x, y;
	Player.GetPosition(&x, &y);

	//STATE_OVERWORLD_01
	if (stateScene == STATE_OVERWORLD_01 && Scene.GetMap()[tile_x + (tile_y*SCENE_WIDTH)].tileId == 19)
		stateScene = STATE_DUNGEON_01;
	else if (stateScene == STATE_OVERWORLD_01 &&  y == 480)
		stateScene = STATE_OVERWORLD_03;
	else if (stateScene == STATE_OVERWORLD_01 && x == 0)
		stateScene = STATE_OVERWORLD_04;
	else if (stateScene == STATE_OVERWORLD_01 && x == 720)
		stateScene = STATE_OVERWORLD_02;

	//STATE_OVERWORLD_02
	else if (stateScene == STATE_OVERWORLD_02 && x == 0)
		stateScene = STATE_OVERWORLD_01;

	//STATE_OVERWORLD_03
	else if (stateScene == STATE_OVERWORLD_03 && y == 0)
		stateScene = STATE_OVERWORLD_01;

	//STATE_OVERWORLD_04
	else if (stateScene == STATE_OVERWORLD_04 && x == 720)
		stateScene = STATE_OVERWORLD_01;

	//STATE_DUNGEON_01
	else if (stateScene == STATE_DUNGEON_01 &&  y == 48)
		stateScene = STATE_OVERWORLD_01;
	else if (stateScene == STATE_DUNGEON_01 &&  y == 432)
		stateScene = STATE_DUNGEON_04;
	else if (stateScene == STATE_DUNGEON_01 &&  x == 48)
		stateScene = STATE_DUNGEON_02;

	//STATE_DUNGEON_02
	else if (stateScene == STATE_DUNGEON_02 &&  x == 672)
		stateScene = STATE_DUNGEON_01;

	//STATE_DUNGEON_03
	else if (stateScene == STATE_DUNGEON_03 &&  x == 48)
		stateScene = STATE_DUNGEON_04;

	//STATE_DUNGEON_04
	else if (stateScene == STATE_DUNGEON_04 &&  y == 48)
		stateScene = STATE_DUNGEON_01;
	else if (stateScene == STATE_DUNGEON_04 &&  y == 432)
		stateScene = STATE_DUNGEON_06;
	else if (stateScene == STATE_DUNGEON_04 &&  x == 672)
		stateScene = STATE_DUNGEON_03;
	else if (stateScene == STATE_DUNGEON_04 &&  x == 48)
		stateScene = STATE_DUNGEON_05;

	//STATE_DUNGEON_05
	else if (stateScene == STATE_DUNGEON_05 &&  x == 672)
		stateScene = STATE_DUNGEON_04;

	//STATE_DUNGEON_06
	else if (stateScene == STATE_DUNGEON_06 &&  y == 48)
		stateScene = STATE_DUNGEON_04;
	else if (stateScene == STATE_DUNGEON_06 &&  x == 48)
		stateScene = STATE_DUNGEON_08;

	//STATE_DUNGEON_07
	else if (stateScene == STATE_DUNGEON_07 &&  x == 672)
		stateScene = STATE_DUNGEON_04;

	//STATE_DUNGEON_08
	else if (stateScene == STATE_DUNGEON_08 &&  y == 48)
		stateScene = STATE_DUNGEON_06;

}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	int numTexture;

	int x, y;

	setSceneState();

	if (lastStateScene != stateScene) {
		switch (stateScene) {
		case STATE_OVERWORLD_01:
			numTexture = OVERWORLD_TILES;
			Scene.LoadOverworldLevel(1);
			if(lastStateScene==STATE_OVERWORLD_02) Player.SetTile(15, 5);
			else if (lastStateScene == STATE_OVERWORLD_03) {
				Player.SetTile(8, 10);
				//cutrissim
				c->Destroy(Scene.GetMap());
				//cutrissim
			}
			else if (lastStateScene == STATE_OVERWORLD_04) {
				Player.SetTile(0, 5);
				//cutrissim
				d->Destroy(Scene.GetMap());
				//cutrissim
			}
			else if (lastStateScene == STATE_DUNGEON_01) {
				Player.SetTile(4, 9);
			}
			break;
		case STATE_OVERWORLD_02:
			numTexture = OVERWORLD_TILES;
			Scene.LoadOverworldLevel(2);
			Player.SetTile(0, 5);
			break;
		case STATE_OVERWORLD_03:
			numTexture = OVERWORLD_TILES;
			Scene.LoadOverworldLevel(3);
			Player.SetTile(8, 0);

			//cutrissim
			c = new cOctorok();
			c->SetTile(4, 5);
			c->UpdateMapTiles(Scene.GetMap(), -1, -1);
			c->SetWidthHeight(TILE_SIZE, TILE_SIZE);
			c->SetState(STATE_LOOKDOWN);
			Enemies.push_back(c);
			//cutrissim
			break;
		case STATE_OVERWORLD_04:
			numTexture = OVERWORLD_TILES;
			Scene.LoadOverworldLevel(4);
			Player.SetTile(15, 5);

			//cutrissim
			d = new cTektike();
			d->SetTile(6, 5);
			d->UpdateMapTiles(Scene.GetMap(), -1, -1);
			d->SetWidthHeight(TILE_SIZE, TILE_SIZE);
			d->SetState(STATE_RESTING);
			d->SetLifes(1);
			Enemies.push_back(d);
			//cutrissim
			break;
		case STATE_DUNGEON_01:
			if (lastStateScene == STATE_OVERWORLD_01) {
				Player.SetTile(8, 1);
			}
			else if (lastStateScene == STATE_DUNGEON_04) {
				Player.SetTile(8, 9);
			}
			else if (lastStateScene == STATE_DUNGEON_02) {
				Player.SetTile(1, 5);
			}

			Scene.LoadDungeonLevel(1);
			numTexture = DUNGEON_TILES;
			break;
		case STATE_DUNGEON_02:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(2);
			Player.SetTile(14, 5);
			break;
		case STATE_DUNGEON_03:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(3);
			Player.SetTile(1, 5);
			break;
		case STATE_DUNGEON_04:
			if (lastStateScene == STATE_DUNGEON_01) {
				Player.SetTile(8, 1);
			}
			else if (lastStateScene == STATE_DUNGEON_05) {
				Player.SetTile(1, 5);
			}
			else if (lastStateScene == STATE_DUNGEON_03) {
				Player.SetTile(14, 5);
			}
			else if (lastStateScene == STATE_DUNGEON_07) {
				Player.SetTile(1, 5);
			}

			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(4);
			break;
		case STATE_DUNGEON_05:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(5);
			Player.SetTile(14, 5);
			break;
		case STATE_DUNGEON_06:
			if (lastStateScene == STATE_DUNGEON_04) {
				Player.SetTile(8, 1);
			}
			else if (lastStateScene == STATE_DUNGEON_08) {
				Player.SetTile(1, 5);
			}
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(6);
			break;
		case STATE_DUNGEON_07:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(7);
			Player.SetTile(14, 5);
			break;
		case STATE_DUNGEON_08:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(8);
			//Player.SetState(STATE_LOOKUP);
			Player.SetTile(8, 1);
			break;
		}
		lastNumTexture = numTexture;
		lastStateScene = stateScene;
	}
	else numTexture = lastNumTexture;

	Scene.Draw(Data.GetID(numTexture));

	Player.Draw(Data.GetID(LINK));
	if (Espasa != NULL) Espasa->Draw(Data.GetID(LINK));
	for (int i = 0; i < Enemies.size(); i++) {
		int w, h;
		Enemies[i]->GetWidthHeight(&w, &h);
		if (w == 2*TILE_SIZE) // It's Ganon
			Enemies[i]->Draw(Data.GetID(BOSSES));
		else Enemies[i]->Draw(Data.GetID(OVERWORLD_ENEMIES));
	}

	createPanel();
	glutSwapBuffers();
}

int cGame::getStateScene()
{
	return stateScene;
}

void cGame::setStateScene(int ss)
{
	stateScene = ss;
}


void cGame::DetectCollisions(std::vector<cBicho*> *bichos) {
	for (int i = 0; i < bichos->size(); i++) {
		cRect rt;
		(*bichos)[i]->GetArea(&rt);
		if (Player.ShieldBlocks((*bichos)[i])) dynamic_cast<cTerrestre*>((*bichos)[i])->Destroy(Scene.GetMap());
		else if (Player.Collides(&rt) && !Player.isImmune()) {
			Player.Hit(&rt);
		}
	}
}

void cGame::ProcessDynamicCollisions() {
	// Detect collisions with enemies
	int x, y;
	Player.GetPosition(&x, &y);
	std::vector<cBicho*> *bichos = &Scene.GetMap()[(y/TILE_SIZE)*SCENE_WIDTH + x / TILE_SIZE].bichos;
	DetectCollisions(bichos);

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

void cGame::ClearEnemiesFromMap() {
	for (int j = 0; j < SCENE_HEIGHT; j++) {
		for (int i = 0; i < SCENE_WIDTH; i++) {
			Scene.GetMap()[j*SCENE_WIDTH + i].bichos.clear();
		}
	}
}