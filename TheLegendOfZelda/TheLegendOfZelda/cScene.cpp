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

bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
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
					fscanf(fd,"%c",&tile[0]);
					fscanf(fd, "%c", &tile[1]);
					
					if(tile[0]==' ') map[(j*SCENE_WIDTH) + i].tileId = atoi(tile);
					else map[(j*SCENE_WIDTH) + i].tileId = atoi(tile);

					map[(j*SCENE_WIDTH) + i].isWall = isAWall(atoi(tile));

					coordx_tile = (float) 1 / 256 + map[(j*SCENE_WIDTH) + i].tileId % 15 * (float) 17 / 256;
					int row = (int) map[(j*SCENE_WIDTH) + i].tileId / 15;
					coordy_tile = (float)1 / 256 + row * (float)17 / 256;

					float tile_size = (float)16 / 256; // Size of the tile in the texture 
					glTexCoord2f(coordx_tile, coordy_tile + tile_size); glVertex2i(px, py);
					glTexCoord2f(coordx_tile + tile_size, coordy_tile + tile_size);    glVertex2i(px + BLOCK_SIZE, py);
					glTexCoord2f(coordx_tile + tile_size, coordy_tile); glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
					glTexCoord2f(coordx_tile, coordy_tile); glVertex2i(px, py + BLOCK_SIZE);

					px+=TILE_SIZE;
				}
				fscanf(fd,"%c",&tile); //pass enter
			}

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