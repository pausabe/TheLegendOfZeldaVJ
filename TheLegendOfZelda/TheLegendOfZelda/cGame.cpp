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
	res = Data.LoadImage(TREASURES, "resources/treasures.png", GL_RGBA);
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
	Player.Logic(Scene.GetMap(), Dungeon);

	int x, y;
	Player.GetPosition(&x, &y);
	if ((x / TILE_SIZE == 8 || x / TILE_SIZE == 7) && (y / TILE_SIZE == SCENE_HEIGHT - 3) && (Dungeon.GetCurrentDungeon() == 1) && Player.HoldsKey(1)) {
		Dungeon.OpenDoor(1);
		Scene.LoadDungeonLevel(2, Dungeon);
	} else if ((x / TILE_SIZE == 8 || x / TILE_SIZE == 7) && (y / TILE_SIZE == SCENE_HEIGHT - 3) && (Dungeon.GetCurrentDungeon() == 5) && Player.HoldsKey(2)) {
		Dungeon.OpenDoor(2);
		Scene.LoadDungeonLevel(6, Dungeon);
	}

	for (int i = 0; i < Objects.size(); i++) {
		cRect rt;
		Objects[i]->GetArea(&rt);
		if (Player.Collides(&rt) && (keys['a'] || keys['A'])) {
			Player.PickObject(Objects[i]);
			Objects.erase(Objects.begin() + i);
		}
	}

	if (Player.HoldsStepladder() && (keys['a'] || keys['A'])) PlaceStepladder();

	if (Espasa == NULL) Espasa = dynamic_cast<cEspasa*>(Player.ThrowProjectil(Scene.GetMap()));
	else if (Espasa->ToBeDestroyed()) Espasa = NULL;
	else Espasa->Logic(Scene.GetMap());

	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->ToBeDestroyed()) {
			cBicho* aux = Enemies[i];

			// Drop object
			cObject* droppingObject = aux->DropsObject();
			if (droppingObject != nullptr) {
				int x, y;
				aux->GetPosition(&x, &y);

				droppingObject->SetPosition(x, y);
				Objects.push_back(droppingObject);
			}
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

void cGame::PlaceStepladder() {
	int tx, ty;
	Player.GetTile(&tx, &ty);
	if (Player.GetState() == STATE_LOOKLEFT || Player.GetState() == STATE_WALKLEFT) tx--;
	else if (Player.GetState() == STATE_LOOKRIGHT || Player.GetState() == STATE_WALKRIGHT) tx++;
	else if (Player.GetState() == STATE_LOOKUP || Player.GetState() == STATE_WALKUP) ty++;
	else if (Player.GetState() == STATE_LOOKDOWN || Player.GetState() == STATE_WALKDOWN)ty--;

	int tileId = Scene.GetMap()[ty*SCENE_WIDTH + tx].tileId;
	if (tileId == -2 || tileId == 22) {
		cStepladder* s = Player.DropStepladder();
		s->SetPosition(tx*TILE_SIZE, ty*TILE_SIZE);
		Objects.push_back(s);
	}
}

void cGame::createPanel()
{
	printHearts();
}

void cGame::printHearts()
{
	int screen_x, screen_y;

	screen_x = SCENE_Xo;
	screen_y = SCENE_Yo + TILE_SIZE * 11;

	float xo, yo, xf, yf;

	float sprite_size = 8;
	float texture_size = 256;


	/*xo = 0;
	yo = 0;
	xf = (float)(sprite_size / texture_size);
	yf = (float)(sprite_size / texture_size);*/

	int w = 32;
	glEnable(GL_TEXTURE_2D);

	int playerLives = Player.GetLifes();
	

	xo = (float)(sprite_size / texture_size) * 2;
	yo = 0;
	xf = (float)(sprite_size / texture_size) * 3;
	yf = (float)(sprite_size / texture_size);

	//primer cor
	if (playerLives > 1) {
		xo = 0;
		yo = 0;
		xf = (float)(sprite_size / texture_size);
		yf = (float)(sprite_size / texture_size);
	}
	else if (playerLives == 1) {
		xo = (float)(sprite_size / texture_size);
		yo = 0;
		xf = (float)(sprite_size / texture_size)*2;
		yf = (float)(sprite_size / texture_size);
	}
	else if(playerLives == 0){
		xo = (float)(sprite_size / texture_size)*2;
		yo = 0;
		xf = (float)(sprite_size / texture_size)*3;
		yf = (float)(sprite_size / texture_size);
	}
	glPushMatrix();
	glTranslatef(550.0f, 15.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, Data.GetID(TREASURES));
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
	glEnd();
	glPopMatrix();


	//segon cor
	if (playerLives > 3) {
		xo = 0;
		yo = 0;
		xf = (float)(sprite_size / texture_size);
		yf = (float)(sprite_size / texture_size);
	}
	else if (playerLives == 3) {
		xo = (float)(sprite_size / texture_size);
		yo = 0;
		xf = (float)(sprite_size / texture_size) * 2;
		yf = (float)(sprite_size / texture_size);
	}
	else if(playerLives < 3) {
		xo = (float)(sprite_size / texture_size) * 2;
		yo = 0;
		xf = (float)(sprite_size / texture_size) * 3;
		yf = (float)(sprite_size / texture_size);
	}
	glPushMatrix();
	glTranslatef(585.0f, 15.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, Data.GetID(TREASURES));
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
	glEnd();
	glPopMatrix();

	//tercer cor
	if (playerLives == 6) {
		xo = 0;
		yo = 0;
		xf = (float)(sprite_size / texture_size);
		yf = (float)(sprite_size / texture_size);
	}
	else if (playerLives == 5) {
		xo = (float)(sprite_size / texture_size);
		yo = 0;
		xf = (float)(sprite_size / texture_size) * 2;
		yf = (float)(sprite_size / texture_size);
	}
	else if (playerLives < 5) {
		xo = (float)(sprite_size / texture_size) * 2;
		yo = 0;
		xf = (float)(sprite_size / texture_size) * 3;
		yf = (float)(sprite_size / texture_size);
	}
	glPushMatrix();
	glTranslatef(620.0f, 15.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, Data.GetID(TREASURES));
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
	glEnd();
	glPopMatrix();



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
	else if (stateScene == STATE_DUNGEON_02 &&  y == 432)
		stateScene = STATE_DUNGEON_05;


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
		stateScene = STATE_DUNGEON_07;	
	else if (stateScene == STATE_DUNGEON_06 &&  y == 432)
		stateScene = STATE_DUNGEON_08;

	//STATE_DUNGEON_07
	else if (stateScene == STATE_DUNGEON_07 &&  x == 672)
		stateScene = STATE_DUNGEON_06;

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

			Scene.LoadDungeonLevel(1, Dungeon);
			numTexture = DUNGEON_TILES;
			break;
		case STATE_DUNGEON_02:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(2, Dungeon);
			Player.SetTile(14, 5);
			break;
		case STATE_DUNGEON_03:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(3, Dungeon);
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
			else if (lastStateScene == STATE_DUNGEON_06) {
				Player.SetTile(8, 9);
			}

			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(4, Dungeon);
			break;
		case STATE_DUNGEON_05:
			if (lastStateScene == STATE_DUNGEON_01) {
				Player.SetTile(14, 5);
			}
			else if (lastStateScene == STATE_DUNGEON_02) {
				Player.SetTile(8, 1);
			}
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(5, Dungeon);
			break;
		case STATE_DUNGEON_06:
			if (lastStateScene == STATE_DUNGEON_04) {
				Player.SetTile(8, 1);
			}
			/*else if (lastStateScene == STATE_DUNGEON_08) {
				Player.SetTile(8, 9);
			}*/
			else if (lastStateScene == STATE_DUNGEON_07) {
				Player.SetTile(1, 5);
			}
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(6, Dungeon);
			break;
		case STATE_DUNGEON_07:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(7, Dungeon);
			Player.SetTile(14, 5);
			break;
		case STATE_DUNGEON_08:
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(8, Dungeon);
			//Player.SetState(STATE_LOOKUP);
			Player.SetTile(8, 1);
			break;
		}
		lastNumTexture = numTexture;
		lastStateScene = stateScene;

		if (numTexture == DUNGEON_TILES) {
			Dungeon.LoadEnemies(stateScene - 15, &Enemies);
			Dungeon.LoadObjects(stateScene - 15, &Objects);
		}
		else {
			LoadOverworldEnemies();
			Dungeon.ExitDungeon();
		}
		ClearEnemiesFromMap();
	}
	else numTexture = lastNumTexture;

	Scene.Draw(Data.GetID(numTexture));

	// Draw player 
	Player.Draw(Data.GetID(LINK));
	// Draw sword, if necessary
	if (Espasa != NULL) Espasa->Draw(Data.GetID(LINK));
	
	// Draw enemies
	for (int i = 0; i < Enemies.size(); i++) {
		int x, y;
		Enemies[i]->GetPosition(&x, &y);
		if (x >= 0 && y >= 0) {
			int w, h;
			Enemies[i]->GetWidthHeight(&w, &h);
			if (w == 2 * TILE_SIZE) // It's Ganon
				Enemies[i]->Draw(Data.GetID(BOSSES));
			else Enemies[i]->Draw(Data.GetID(OVERWORLD_ENEMIES));
		}
	}

	// Draw objects
	for (int i = 0; i < Objects.size(); i++) {
		Objects[i]->Draw(Data.GetID(TREASURES));
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

void cGame::LoadOverworldEnemies() {
	
}