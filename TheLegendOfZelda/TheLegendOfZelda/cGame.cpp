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

	//createPanel();
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
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10.0f);

	glPushMatrix();
	//glRotatef(alfa, 1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(2.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(4.0f, -1.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//glRotatef(2.0f*alfa, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.0f, 2.0f, 0.0f);
	glVertex3f(-0.0f, 2.1f, 0.0f);
	glVertex3f(-0.0f, 2.2f, 0.0f);
	glVertex3f(-0.0f, 2.3f, 0.0f);
	glVertex3f(-0.0f, 2.4f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 3.0f, 0.0f);
	glVertex3f(-1.0f, 3.5f, 0.0f);
	glVertex3f(1.0f, 3.5f, 0.0f);
	glEnd();

	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, (alfa / 200.0f));
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-3.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-3.0f, -1.0f, 0.0f);
	glColor3f(0.5f, 1.5f, 0.5f);
	glVertex3f(-4.0f, 0.5f, 0.0f);
	glColor3f(1.5f, 0.5f, 0.5f);
	glVertex3f(-4.0f, -1.5f, 0.0f);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();

}

void cGame::setSceneState() {
	int tile_x, tile_y;
	Player.GetTile(&tile_x, &tile_y);

	int x, y;
	Player.GetPosition(&x, &y);

	if (stateScene == STATE_OVERWORLD_01 && Scene.GetMap()[tile_x + (tile_y*SCENE_WIDTH)].tileId == 19)
		stateScene = STATE_DUNGEON_01;
	else if (stateScene == STATE_DUNGEON_01 &&  y == 48)
		stateScene = STATE_OVERWORLD_01;
	else if (stateScene == STATE_OVERWORLD_01 &&  y == 480)
		stateScene = STATE_OVERWORLD_03;
	else if (stateScene == STATE_OVERWORLD_01 && x == 0)
		stateScene = STATE_OVERWORLD_04;
	else if (stateScene == STATE_OVERWORLD_01 && x == 720)
		stateScene = STATE_OVERWORLD_02;
	else if (stateScene == STATE_OVERWORLD_02 && x == 0)
		stateScene = STATE_OVERWORLD_01;
	else if (stateScene == STATE_OVERWORLD_03 && y == 0)
		stateScene = STATE_OVERWORLD_01;
	else if (stateScene == STATE_OVERWORLD_04 && x == 720)
		stateScene = STATE_OVERWORLD_01;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	//createPanel();

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
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(1);
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