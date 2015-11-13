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

	PlaySound(TEXT("sounds/Overworld_Theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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
	res = Data.LoadImage(MENU, "resources/menu.png", GL_RGBA);
	if (!res) return false;


	stateScene = STATE_OVERWORLD_01;
	lastStateScene = STATE_OVERWORLD_01;
	lastNumTexture = OVERWORLD_TILES;

	Scene.LoadOverworldLevel(1);


	//Player initialization
	Player.SetTile(8, 5);
	Player.SetWidthHeight(TILE_SIZE, TILE_SIZE);
	Player.SetState(STATE_LOOKDOWN);
	Player.SetLifes(6);

	
	// Overworld enemies initialization
	OverworldEnemies[0] = new std::vector<cBicho*>();
	OverworldEnemies[1] = new std::vector<cBicho*>();
	OverworldEnemies[2] = new std::vector<cBicho*>();
	OverworldEnemies[3] = new std::vector<cBicho*>();

	cOctorok* o1 = new cOctorok();
	o1->SetTile(4, 5);
	o1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o1->SetState(STATE_LOOKDOWN);
	o1->SetLifes(1);
	OverworldEnemies[1]->push_back(o1);

	o1 = new cOctorok();
	o1->SetTile(8, 2);
	o1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o1->SetState(STATE_LOOKDOWN);
	o1->SetLifes(1);
	OverworldEnemies[1]->push_back(o1);

	o1 = new cOctorok();
	o1->SetTile(13, 9);
	o1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o1->SetState(STATE_LOOKDOWN);
	o1->SetLifes(1);
	OverworldEnemies[1]->push_back(o1);

	o1 = new cOctorok();
	o1->SetTile(4, 9);
	o1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o1->SetState(STATE_LOOKDOWN);
	o1->SetLifes(1);
	OverworldEnemies[1]->push_back(o1);

	o1 = new cOctorok();
	o1->SetTile(10, 7);
	o1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	o1->SetState(STATE_LOOKDOWN);
	o1->SetLifes(1);
	OverworldEnemies[1]->push_back(o1);

	cTektike* d1 = new cTektike();
	d1->SetTile(11, 7);
	d1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d1->SetState(STATE_RESTING);
	d1->SetLifes(1);
	OverworldEnemies[2]->push_back(d1);
	
	d1 = new cTektike();
	d1->SetTile(4, 2);
	d1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d1->SetState(STATE_RESTING);
	d1->SetLifes(1);
	OverworldEnemies[2]->push_back(d1);

	d1 = new cTektike();
	d1->SetTile(3, 10);
	d1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d1->SetState(STATE_RESTING);
	d1->SetLifes(1);
	OverworldEnemies[2]->push_back(d1);

	d1 = new cTektike();
	d1->SetTile(6, 5);
	d1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d1->SetState(STATE_RESTING);
	d1->SetLifes(1);
	OverworldEnemies[2]->push_back(d1);

	d1 = new cTektike();
	d1->SetTile(13, 10);
	d1->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d1->SetState(STATE_RESTING);
	d1->SetLifes(1);
	OverworldEnemies[2]->push_back(d1);

	cDarknut* d = new cDarknut();
	d->SetTile(5, 8);
	d->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d->SetState(STATE_LOOKDOWN);
	d->SetLifes(4);
	OverworldEnemies[3]->push_back(d);

	d = new cDarknut();
	d->SetTile(7, 3);
	d->SetWidthHeight(TILE_SIZE, TILE_SIZE);
	d->SetState(STATE_LOOKDOWN);
	d->SetLifes(4);
	OverworldEnemies[3]->push_back(d);

	LoadOverworldEnemies();
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

	if (Player.GetLifes() == 0) return res;

	if (!Player.isJumping()) {
		if ((keys['s'] || keys['S'])&& !sKeyPressed) {
			
			if(Player.GetLifes()==6) PlaySound(TEXT("sounds/LOZ_Sword_Combined.wav"), NULL, SND_FILENAME | SND_ASYNC);
			else PlaySound(TEXT("sounds/LOZ_Sword.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
	if (!keys['a']) aKeyPressed = false;


	//Game Logic
	Player.Logic(Scene.GetMap(), Dungeon);

	int x, y;
	Player.GetPosition(&x, &y);
	if ((x / TILE_SIZE == 8 || x / TILE_SIZE == 7) && (y / TILE_SIZE == SCENE_HEIGHT - 3) && (Dungeon.GetCurrentDungeon() == 1) && Player.HoldsKey(1)) {
		PlaySound(TEXT("sounds/LOZ_Unlock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		Dungeon.OpenDoor(1);
		Scene.LoadDungeonLevel(2, Dungeon);
	} else if ((x / TILE_SIZE == 8 || x / TILE_SIZE == 7) && (y / TILE_SIZE == SCENE_HEIGHT - 3) && (Dungeon.GetCurrentDungeon() == 5) && Player.HoldsKey(2)) {
		Dungeon.OpenDoor(2);
		Scene.LoadDungeonLevel(6, Dungeon);
	}

	for (int i = 0; i < Objects.size(); i++) {
		cRect rt;
		Objects[i]->GetArea(&rt);
		if (Player.Collides(&rt) && (keys['a'] || keys['A']) && !aKeyPressed) {
			int x, y;
			Objects[i]->GetPosition(&x, &y);
			int px, py;
			Player.GetPosition(&px, &py);
			if (Objects[i]->GetKeyId() != -1 || !(x / TILE_SIZE == px / TILE_SIZE && y / TILE_SIZE == py / TILE_SIZE)) {
				Player.PickObject(Objects[i]);
				PlaySound(TEXT("sounds/LOZ_Key.wav"), NULL, SND_FILENAME | SND_ASYNC);

				if (Objects[i]->GetKeyId() == -1) {
					Scene.GetMap()[(y / TILE_SIZE)*SCENE_WIDTH + x / TILE_SIZE].isWall = wallUnderStepladder;
				}

				Objects.erase(Objects.begin() + i);
				aKeyPressed = true;
			}
		}
	}

	if (Tes != NULL) {
		Tes->Logic(Scene.GetMap(), Enemies);
		if (Tes->ToBeDestroyed()) {
			Tes = NULL;
			tesCooldown = TES_COOLDOWN;
		}
	}

	if (tesCooldown > 0) tesCooldown--;
	if (Tes == NULL && tesCooldown == 0 && keys['t']) {
		Tes = new cTes();
		Tes->SetPosition(Player.GetPosX(), Player.GetPosY());
		Tes->SetWidthHeight(TILE_SIZE, TILE_SIZE);
		Tes->SetState(STATE_LOOKDOWN);
	}

	if (Player.HoldsStepladder() && (keys['a'] || keys['A']) && !aKeyPressed) {
		PlaceStepladder();
		aKeyPressed = true;
	}

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
		wallUnderStepladder = Scene.GetMap()[(ty)*SCENE_WIDTH + tx].isWall;

		cStepladder* s = Player.DropStepladder();
		s->SetPosition(tx*TILE_SIZE, ty*TILE_SIZE);
		Scene.GetMap()[ty*SCENE_WIDTH + tx].isWall = false;
		Objects.push_back(s);
	}
}

void cGame::createPanel()
{
	printHearts();
	printObjectBox();
	printMap();
}

void cGame::printMapRoom(int screen_x, int screen_y, int state) {
	glBegin(GL_QUADS);
	glVertex2i(screen_x, screen_y);
	glVertex2i(screen_x + 80, screen_y);
	glVertex2i(screen_x + 80, screen_y + 40);
	glVertex2i(screen_x, screen_y + 40);
	glEnd();
	if (stateScene == state) {
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(screen_x + 80 * ((float)Player.GetPosX() / (16 * TILE_SIZE)), screen_y + 40 * ((float)Player.GetPosY() / (11 * TILE_SIZE)));
		glVertex2i(0, 0);
		glColor3f(0.4f, 0.4f, 0.4f);
		glEnd();
	}

}

void cGame::printMap() {
	
	int screen_x, screen_y;

	screen_x = SCENE_Xo;
	screen_y = SCENE_Yo + TILE_SIZE * 11;
	
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glColor3f(0.4f, 0.4f, 0.4f);

	if (stateScene > 14) {
		glTranslatef(30.0f, 0.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_02);
		glTranslatef(0.0f, 44.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_05);
		glTranslatef(0.0f, 44.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_07);
		glTranslatef(84.0f, -88.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_01);
		glTranslatef(0.0f, 44.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_04);
		glTranslatef(0.0f, 44.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_06);
		glTranslatef(0.0f, 44.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_08);
		glTranslatef(84.0f, -88.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_DUNGEON_03);

	}
	else {
		glTranslatef(30.0f, 15.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_OVERWORLD_04);
		glTranslatef(84.0f, 0.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_OVERWORLD_01);
		glTranslatef(84.0f, 0.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_OVERWORLD_02);
		glTranslatef(-84.0f, 44.0f, 0.0f);
		printMapRoom(screen_x, screen_y, STATE_OVERWORLD_03);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);

}

void cGame::printObjectBox()
{
	int screen_x, screen_y;

	screen_x = SCENE_Xo;
	screen_y = SCENE_Yo + TILE_SIZE * 11;

	float xo, yo, xf, yf;

	float sprite_size = 38;
	float texture_size = 512;

	int w = 80;
	glEnable(GL_TEXTURE_2D);

	xo = 0;
	yo = 0;
	xf = (float)(40.0f / texture_size);
	yf = (float)(40.0f / texture_size);

	glPushMatrix();
		glTranslatef(330.0f, 15.0f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, Data.GetID(MENU));
		glBegin(GL_QUADS);
			glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
			glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
			glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
			glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
		glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(430.0f, 15.0f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, Data.GetID(MENU));
		glBegin(GL_QUADS);
			glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
			glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
			glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
			glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
		glEnd();
	glPopMatrix();

	//LIFE title
	w = 80;

	xo = (float)(80.0f / texture_size);;
	yo = 0;
	xf = (float)(170.0f / texture_size);
	yf = (float)(50.0f / texture_size);

	glPushMatrix();
		glTranslatef(580.0f, 50.0f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, Data.GetID(MENU));
		glBegin(GL_QUADS);
			glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
			glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
			glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
			glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
		glEnd();
	glPopMatrix();

	if (Player.HoldsKey(1) || Player.HoldsKey(2)) {
		w = 40;

		xo = (float)(40.0f / texture_size);;
		yo = 0;
		xf = (float)(55.0f / texture_size);
		yf = (float)(20.0f / texture_size);

		glPushMatrix();
			glTranslatef(348.0f, 34.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, Data.GetID(MENU));
			glBegin(GL_QUADS);
				glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
				glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
				glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
				glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
			glEnd();
		glPopMatrix();
	}

	if (Player.HoldsStepladder()) {
		w = 32;

		xo = (float)(59.0f / texture_size);;
		yo = 0;
		xf = (float)(75.0f / texture_size);
		yf = (float)(16.0f / texture_size);

		glPushMatrix();
		glTranslatef(455.0f, 38.0f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, Data.GetID(MENU));
		glBegin(GL_QUADS);
		glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y);
		glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y);
		glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y + w);
		glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y + w);
		glEnd();
		glPopMatrix();
	}

}

void cGame::printHearts()
{
	int screen_x, screen_y;

	screen_x = SCENE_Xo;
	screen_y = SCENE_Yo + TILE_SIZE * 11;

	float xo, yo, xf, yf;

	float sprite_size = 8;
	float texture_size = 256;

	int w = 32;
	glEnable(GL_TEXTURE_2D);

	//Player.SetLifes(6);
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
	glTranslatef(570.0f, 15.0f, 0.0f);

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
	glTranslatef(605.0f, 15.0f, 0.0f);

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
	glTranslatef(640.0f, 15.0f, 0.0f);

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
	if (stateScene == STATE_OVERWORLD_01 && Scene.GetMap()[tile_x + (tile_y*SCENE_WIDTH)].tileId == 19) {
		stateScene = STATE_DUNGEON_08;

		PlaySound(NULL,0,0);
		PlaySound(TEXT("sounds/Underworld_Theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
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
	else if (stateScene == STATE_DUNGEON_01 &&  y == 48) {
		stateScene = STATE_OVERWORLD_01;
		PlaySound(NULL, 0, 0);
		PlaySound(TEXT("sounds/Overworld_Theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
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
	else if (stateScene == STATE_DUNGEON_05 && y == 48)
		stateScene = STATE_DUNGEON_02;

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

	if (Player.GetLifes() == 0) {
		if (gameOver > 0) {
			gameOver--;
			float aux = ((float)gameOver / (float)GAME_OVER_ANIMATION);
			glColor3f(aux, aux, aux);
		} else if (gameOver == 0) {
			glDisable(GL_TEXTURE_2D);

			glColor3f(1.0f, 0.2f, 0.2f);
			char c[] = "VALAR MORGHULIS";
			glRasterPos2f(320, (float)GAME_HEIGHT/2);

			for (int i = 0; i < 16; i++) 
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[i]);
			glEnable(GL_TEXTURE_2D);
			glutSwapBuffers();
			return;
		}
	}
	
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
			}
			else if (lastStateScene == STATE_OVERWORLD_04) {
				Player.SetTile(0, 5);
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

			break;
		case STATE_OVERWORLD_04:
			numTexture = OVERWORLD_TILES;
			Scene.LoadOverworldLevel(4);
			Player.SetTile(15, 5);

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
			if (lastStateScene == STATE_DUNGEON_01) {
				Player.SetTile(14, 5);
			}
			else if (lastStateScene == STATE_DUNGEON_05) {
				Player.SetTile(8, 9);
			}
			numTexture = DUNGEON_TILES;
			Scene.LoadDungeonLevel(2, Dungeon);
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
			else if (lastStateScene == STATE_DUNGEON_04) {
				Player.SetTile(14, 5);
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
		if (Tes != NULL) Tes->SetPosition(Player.GetPosX(), Player.GetPosY());
		ClearEnemiesFromMap();
	}
	else numTexture = lastNumTexture;

	Scene.Draw(Data.GetID(numTexture));

	// Draw objects
	for (int i = 0; i < Objects.size(); i++) {
		int x, y;
		Objects[i]->GetPosition(&x, &y);
		if (x >= 0 && y >= 0) {
			Objects[i]->Draw(Data.GetID(TREASURES));
		}
	}

	// Draw player 
	Player.Draw(Data.GetID(LINK));

	// Draw Tes, if necessary
	if (Tes != NULL) Tes->Draw(Data.GetID(LINK));

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
	Enemies.assign(OverworldEnemies[stateScene - 5]->begin(), OverworldEnemies[stateScene - 5]->end());
}