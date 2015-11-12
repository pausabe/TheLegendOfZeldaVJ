#include "cStepladder.h"

cStepladder::cStepladder() {
}
cStepladder::~cStepladder() {}

void cStepladder::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 256;


	xo = (float)( 208/ texture_size);
	yo = 0.0f;
    xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);


	DrawRect(tex_id, xo, yf, xf, yo);

}
