#include "cScene.h"

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::isAWall(int tile) {
	bool walls[] = {false,true,false,true,true,true,false,true,false,true,true,true,false,true,false,
					false,true,true,true,false,true,false,true,true,true,false,true,false,true,true,
					true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,
					true,true,true,true,false,true,true,true,true,true,false,true,true,true,true,
					true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,					
					true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,
					true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,
					false,false,false,false,true,false,false,false,false,false,true,false,false,false,false,
					true,true,true,false,true,false,true,false,true,true,true,true,true,false,true,
					false,false,false,false,false,false,false,false,false};
	return walls[tile];
}

bool cScene::LoadOverworldLevel(int level)
{
	bool res;
	FILE *fd;
	char file[32];
	int i,j,px,py;
	char auxT[3];
	char tile[2];
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%c",&auxT[0]);
					fscanf(fd, "%c", &auxT[1]);
					if(auxT[1] != ',') fscanf(fd, "%c", &auxT[2]);

						//if(tile[0]==' ') map[(j*SCENE_WIDTH) + i].tileId = atoi(tile);
						//else map[(j*SCENE_WIDTH) + i].tileId = atoi(tile);

					tile[0] = auxT[0];
					if (auxT[1] != ',') 
						tile[1] = auxT[1];
					else {
						tile[0] = '0';
						tile[1] = auxT[0];
					}

					int caca = atoi(tile) - 1;

					map[(j*SCENE_WIDTH) + i].tileId = atoi(tile)-1;

					map[(j*SCENE_WIDTH) + i].isWall = isAWall(atoi(tile)-1);

					coordx_tile = (float) 1 / 256 + map[(j*SCENE_WIDTH) + i].tileId % 15 * (float) 17 / 256;
					int row = (int) map[(j*SCENE_WIDTH) + i].tileId / 15;
					coordy_tile = (float)1 / 256 + row * (float)17 / 256;

					float tile_size = (float)16 / 256; // Size of the tile in the texture 
					glTexCoord2f(coordx_tile, coordy_tile + tile_size); glVertex2i(px, py);
					glTexCoord2f(coordx_tile + tile_size, coordy_tile + tile_size);   glVertex2i(px + BLOCK_SIZE, py);
					glTexCoord2f(coordx_tile + tile_size, coordy_tile);    glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
					glTexCoord2f(coordx_tile, coordy_tile);   glVertex2i(px, py + BLOCK_SIZE);

					px+=TILE_SIZE;
				}
				fscanf(fd,"%c",&tile); //pass enter
			}

		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

bool cScene::isADungeonWall(int tile) {
	return true;
}

bool cScene::LoadDungeonLevel(int level)
{


	bool res;
	FILE *fd;
	char file[32];
	char l,t,r,b;
	int i, j, px, py;
	char tile[2];
	float coordx_tile, coordy_tile;

	res = true;

	if (level<10) sprintf(file, "%s0%d%s", (char *)DUNGEON_FILENAME, level, (char *)FILENAME_EXT);
	else		 sprintf(file, "%s%d%s", (char *)DUNGEON_FILENAME, level, (char *)FILENAME_EXT);

	fd = fopen(file, "r");
	if (fd == NULL) return false;

	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);

	glTexCoord2f((float)239 / 512, (float)171 / 512); glVertex2i((float)TILE_SIZE * 16 - (float)TILE_SIZE * 8 / 16, (float)TILE_SIZE * 11 - (float)TILE_SIZE * 8 / 16);
	glTexCoord2f((float)239 / 512, (float)12 / 512); 	glVertex2i((float)TILE_SIZE * 16 - (float)TILE_SIZE * 8 / 16, (float)TILE_SIZE * 8 / 16);
	glTexCoord2f(0.0f, (float)12 / 512);  glVertex2i((float)TILE_SIZE * 8 / 16, (float)TILE_SIZE * 8 / 16);
	glTexCoord2f(0.0f, (float)171 / 512);  glVertex2i((float)TILE_SIZE * 8 / 16, (float)TILE_SIZE * 11 - (float)TILE_SIZE * 8 / 16);


	
	// Draw doors and similar
	// LEFT
	fscanf(fd, "%c", &l);
	if (l != ' ') {

		coordx_tile = (float)324/512;
		coordy_tile = atoi(&l)*(float)32/512;
		int tile_height = 32;
		int tile_width = 20;
		px = TILE_SIZE*(float)12/16;
		py = TILE_SIZE*(float)72/16;
		glTexCoord2f(coordx_tile, coordy_tile + (float) tile_height/512); glVertex2i(px, py + TILE_SIZE*(float)(tile_height+2)/16);
		glTexCoord2f(coordx_tile + (float)tile_width/512, coordy_tile + (float)tile_height/512);    glVertex2i(px + TILE_SIZE*(float)tile_width/16, py + TILE_SIZE*(float)(tile_height+2) / 16);
		glTexCoord2f(coordx_tile + (float)tile_width/512, coordy_tile); glVertex2i(px + TILE_SIZE*(float)tile_width / 16, py);
		glTexCoord2f(coordx_tile, coordy_tile); glVertex2i(px, py);

	}

	// TOP
	fscanf(fd, "%c", &t);
	if (t != ' ') {
		coordx_tile = (float)272 / 512;
		coordy_tile = atoi(&t)*(float)20 / 512;
		int tile_height = 20;
		int tile_width = 32;
		px = TILE_SIZE*(float)112 / 16;
		py = TILE_SIZE*((float)(33 + 16 * 7) / 16);
		glTexCoord2f(coordx_tile, coordy_tile + (float)tile_height / 512); glVertex2i(px, py + TILE_SIZE*(float)tile_height / 16);
		glTexCoord2f(coordx_tile + (float)tile_width / 512, coordy_tile + (float)tile_height / 512);    glVertex2i(px + TILE_SIZE*(float)(tile_width+1)/ 16, py + TILE_SIZE*(float)tile_height / 16);
		glTexCoord2f(coordx_tile + (float)tile_width / 512, coordy_tile); glVertex2i(px + TILE_SIZE*(float)(tile_width+1) / 16, py);
		glTexCoord2f(coordx_tile, coordy_tile); glVertex2i(px, py);

	}

	// RIGHT
	fscanf(fd, "%c", &r);
	if (r != ' ') {
		coordx_tile = (float)304 / 512;
		coordy_tile = atoi(&r)*(float)32 / 512;
		int tile_height = 32;
		int tile_width = 20;
		px = TILE_SIZE*((float)13 + 20 + 12*16) / 16;
		py = TILE_SIZE*(float)72 / 16;
		glTexCoord2f(coordx_tile, coordy_tile + (float)tile_height / 512); glVertex2i(px, py + TILE_SIZE*(float)(tile_height + 2) / 16);
		glTexCoord2f(coordx_tile + (float)tile_width / 512, coordy_tile + (float)tile_height / 512);    glVertex2i(px + TILE_SIZE*(float)tile_width / 16, py + TILE_SIZE*(float)(tile_height + 2) / 16);
		glTexCoord2f(coordx_tile + (float)tile_width / 512, coordy_tile); glVertex2i(px + TILE_SIZE*(float)tile_width / 16, py);
		glTexCoord2f(coordx_tile, coordy_tile); glVertex2i(px, py);

	}

	// BOTTOM
	fscanf(fd, "%c", &b);
	if (b != ' ') {
		coordx_tile = (float)240 / 512;
		coordy_tile = atoi(&b)*(float)20 / 512;
		int tile_height = 20;
		int tile_width = 32;
		px = TILE_SIZE*(float)112 / 16;
		py = TILE_SIZE*((float) 12 / 16);
		glTexCoord2f(coordx_tile, coordy_tile + (float)tile_height / 512); glVertex2i(px, py + TILE_SIZE*(float)tile_height / 16);
		glTexCoord2f(coordx_tile + (float)tile_width / 512, coordy_tile + (float)tile_height / 512);    glVertex2i(px + TILE_SIZE*(float)(tile_width + 1) / 16, py + TILE_SIZE*(float)tile_height / 16);
		glTexCoord2f(coordx_tile + (float)tile_width / 512, coordy_tile); glVertex2i(px + TILE_SIZE*(float)(tile_width + 1) / 16, py);
		glTexCoord2f(coordx_tile, coordy_tile); glVertex2i(px, py);

	}

	
	fscanf(fd, "%c", &tile[0]);
	for (j = SCENE_HEIGHT - 4; j >= 0; j--)
	{
		px = SCENE_Xo + (float)TILE_SIZE * 2;
		py = SCENE_Yo + (j * TILE_SIZE) + TILE_SIZE;

		for (i = 0; i < SCENE_WIDTH - 4; i++)
		{
			fscanf(fd, "%c", &tile[0]);
			fscanf(fd, "%c", &tile[1]);

			if (tile[0] != ' ' && tile[1] != ' ') {
				map[((j+1)*SCENE_WIDTH) + i + 2].tileId = atoi(tile);
				map[((j+1)*SCENE_WIDTH) + i + 2].isWall = isADungeonWall(atoi(tile));

				int tileId = atoi(tile);

				if (tileId != -1) {

					float tile_height = (float)16 / 512; // Size of the tile in the texture 
					float tile_width = (float)16 / 512; // Size of the tile in the texture 
					float x0 = (float)240 / 512;
					float y0 = 0.0;
					int auxTile = tileId;
					float w = 1, h = 1;
					int columns = 4;
					if (tileId <= 11) {
						tile_height = (float)20 / 512;
						h = 1.25f;
					}
					else if (tileId > 11 && tileId <= 28) {
						auxTile = tileId - 12;
						y0 = (float)60 / 512;
					}
					else {
						auxTile = tileId - 29;
						x0 += (float)16 * 4 / 512;
						tile_width = (float)20 / 512;
						w = 1.25f;
					}
					coordx_tile = x0 + auxTile % columns * tile_width;// (float)1 / 256 + map[(j*SCENE_WIDTH) + i].tileId % 15 * (float)17 / 256;
					coordy_tile = y0 + (int)auxTile / columns * tile_height;// row * (float)17 / 256;

					glTexCoord2f(coordx_tile, coordy_tile + tile_height); glVertex2i(px + BLOCK_SIZE, py);
					glTexCoord2f(coordx_tile + tile_width, coordy_tile + tile_height);   glVertex2i(px, py); 
					glTexCoord2f(coordx_tile + tile_width, coordy_tile); glVertex2i(px, py + BLOCK_SIZE);
					glTexCoord2f(coordx_tile, coordy_tile); glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
				}
			}
			else {
				map[((j + 1)*SCENE_WIDTH) + i + 2].tileId = -1;
				map[((j + 1)*SCENE_WIDTH) + i + 2].isWall = false;
			}
			px += TILE_SIZE;
		}
		fscanf(fd, "%c", &tile); //pass enter

	}
	// Fill the map with the dungeons walls

	for (int i = 0; i < SCENE_WIDTH - 1; i++) {
		map[SCENE_WIDTH + i].isWall = true;
		map[SCENE_WIDTH + i].tileId = -1;
		
		map[((SCENE_HEIGHT - 2)*SCENE_WIDTH) + i].isWall = true;
		map[((SCENE_HEIGHT - 2)*SCENE_WIDTH) + i].tileId = -1;
		
	}

	for (int j = 0; j < SCENE_HEIGHT - 2; j++) {
		map[(j*SCENE_WIDTH) + 1].isWall = true;
		map[(j*SCENE_WIDTH) + 1].tileId = -1;

		map[(j*SCENE_WIDTH) + SCENE_WIDTH - 2].isWall = true;
		map[(j*SCENE_WIDTH) + SCENE_WIDTH - 2].tileId = -1;
	}

	// Open the gates if necessary	
	// Left
	map[5 * SCENE_WIDTH + 1].tileId = -1;
	map[5 * SCENE_WIDTH + 1].isWall = atoi(&l) != 0;
	
	// Top
	map[(SCENE_HEIGHT - 2) * SCENE_WIDTH + 7].tileId = -1;
	map[(SCENE_HEIGHT - 2) * SCENE_WIDTH + 7].isWall = atoi(&t) != 0;
	map[(SCENE_HEIGHT - 2) * SCENE_WIDTH + 8].tileId = -1;
	map[(SCENE_HEIGHT - 2) * SCENE_WIDTH + 8].isWall = atoi(&t) != 0;
	
	// Bottom
	map[SCENE_WIDTH + 7].tileId = -1;
	map[SCENE_WIDTH + 7].isWall = atoi(&t) != 0;
	map[SCENE_WIDTH + 8].tileId = -1;
	map[SCENE_WIDTH + 8].isWall = atoi(&t) != 0;

	// Right
	map[5 * SCENE_WIDTH + SCENE_WIDTH - 2].tileId = -1;
	map[5 * SCENE_WIDTH + SCENE_WIDTH - 2].isWall = atoi(&r) != 0;


	glEnd();
	glEndList();

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}

Tile* cScene::GetMap()
{
	return map;
}