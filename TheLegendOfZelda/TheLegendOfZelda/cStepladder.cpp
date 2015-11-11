#include "cStepladder.h"

cStepladder::cStepladder() {
}
cStepladder::~cStepladder() {}

void cStepladder::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 512;

	if (!deadSword) {
		float xxx, yyy;
		yyy = 194;

		int state = GetState();

		if (state == STATE_ATACKDOWN || state == STATE_LOOKDOWN || state == STATE_WALKDOWN) xxx = 0;
		else if (state == STATE_ATACKUP || state == STATE_LOOKUP || state == STATE_WALKUP) xxx = 60;
		else if (state == STATE_ATACKRIGHT || state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT) xxx = 90;
		else if (state == STATE_ATACKLEFT || state == STATE_LOOKLEFT || state == STATE_WALKLEFT) xxx = 30;

		xo = (float)(xxx / texture_size);
		yo = ((float)(yyy / texture_size));
		xf = xo + (float)(sprite_size / texture_size);
		yf = yo + (float)(sprite_size / texture_size);

		DrawRect(tex_id, xo, yf, xf, yo);
	}
	else {
		float xxx = 178;
		float yyy = 282;

		float xxx2 = 196;
		float yyy2 = 302;

		xo = (float)(xxx / texture_size);
		yo = ((float)(yyy / texture_size));
		xf = (float)(xxx2 / texture_size);
		yf = (float)(yyy2 / texture_size);

		DrawRect(tex_id, xo, yf, xf, yo);
	}
}



void cStepladder::SetPosition(int posx, int posy)
{
	x = posx;
	y = posy;
}
void cStepladder::GetPosition(int *posx, int *posy)
{
	*posx = x;
	*posy = y;
}

