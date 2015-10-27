
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	float sprite_size = 16;
    float texture_size = 512;

	//hi ha 14 pixels en blanc que hem d'anar saltant
	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	xo = (float) ((sprite_size+14)/texture_size);	yo = 0.0f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = (float)((sprite_size + 14)/texture_size)*3;	yo = 0.0f;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = (float)((sprite_size + 14) / texture_size); 
								yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = (float)((sprite_size + 14) / texture_size) * 3;
								yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
								NextFrame(2);
								break;

		case STATE_LOOKUP:		xo = (float)((sprite_size + 14) / texture_size) * 2;	yo = 0.0f;
								break;

		case STATE_LOOKDOWN:	xo = 0.0f;	yo = 0.0f;
								break;

		case STATE_WALKUP:		xo = (float)((sprite_size + 14) / texture_size) * 2;
								yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
								NextFrame(2);
								break;

		case STATE_WALKDOWN:	xo = 0.0f;	
								yo = 0.0f + (GetFrame()*((float)((sprite_size + 14) / texture_size)));
								NextFrame(2);
								break;
	}


	xf = xo + (float)(sprite_size / texture_size);
	yf = yo + (float)(sprite_size / texture_size);

	DrawRect(tex_id,xo,yf,xf,yo);
}
