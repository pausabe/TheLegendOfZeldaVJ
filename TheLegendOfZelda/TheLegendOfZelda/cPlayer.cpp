
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	float sprite_size = 16;
    float texture_size = 512;

	//hi ha 14 pixels en blanc que hem d'anar saltant
	
	bool atack = false;
	int SA;

	switch (GetState())
	{
		case STATE_LOOKLEFT:	
			xo = (float)((sprite_size + 14) / texture_size);	
			yo = 0.0f;
			break;
			
		case STATE_LOOKRIGHT:	
			xo = (float)((sprite_size + 14) / texture_size) * 3;	
			yo = 0.0f;
			break;
			
		case STATE_WALKLEFT:	
			xo = (float)((sprite_size + 14) / texture_size);
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;
			
		case STATE_WALKRIGHT:	
			xo = (float)((sprite_size + 14) / texture_size) * 3;			
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));			
			NextFrame(2);
			break;

		case STATE_LOOKUP:		
			xo = (float)((sprite_size + 14) / texture_size) * 2;	
			yo = 0.0f;
			break;

		case STATE_LOOKDOWN:	
			xo = 0.0f;	
			yo = 0.0f;
			break;

		case STATE_WALKUP:		
			xo = (float)((sprite_size + 14) / texture_size) * 2;
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;

		case STATE_WALKDOWN:	
			xo = 0.0f;
			yo = (GetFrame()*((float)((sprite_size + 14) / texture_size)));
			NextFrame(2);
			break;

		case STATE_ATACKDOWN:	
			AtackFrame();

			SA = GetAtackFrame();
			xo = 0.0f;
			if (SA == 0) {
				yo = (float)((sprite_size + 14) / texture_size) * 2;
			}
			else if (SA == 1) {
				yo = ((float)((sprite_size + 14) / texture_size)) * 2 + ((float)((sprite_size + 8) / texture_size));
			}
			else {
				yo = 0.0f;
			}

			break;
		}


	xf = xo + (float)(sprite_size / texture_size);
	if (GetState() == STATE_ATACKDOWN && SA==1) yf = yo + (float)((sprite_size + 11.0f) / texture_size);
	else yf = yo + (float)(sprite_size / texture_size);


	DrawRect(tex_id,xo,yf,xf,yo);
}
