#include "cObject.h"

cObject::cObject() {
}
cObject::~cObject() {}


void cObject::SetPosition(int posx, int posy)
{
	x = posx;
	y = posy;
}
void cObject::GetPosition(int *posx, int *posy)
{
	*posx = x;
	*posy = y;
}

void cObject::DrawRect(int tex_id, float xo, float yo, float xf, float yf)
{
	int screen_x, screen_y;

	screen_x = x;
	screen_y = y; // +SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);


	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
	glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y);
	glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y + h);
	glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cObject::GetArea(cRect *rc)
{
	rc->left = x;
	rc->right = x + w;
	rc->bottom = y;
	rc->top = y + h;
}
