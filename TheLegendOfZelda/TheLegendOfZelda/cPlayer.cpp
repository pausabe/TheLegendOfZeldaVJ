
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
			xo = 0.0f;
			yo = ((float)((sprite_size + 14) / texture_size)) * 2 + ((float)((sprite_size + 8) / texture_size));
			break;
		
		case STATE_ATACKUP:
			xo = (float)((sprite_size + 14) / texture_size) * 2;
			yo = ((float)((sprite_size + 14) / texture_size)) * 2 + ((float)((sprite_size + 8) / texture_size));
			break;

		case STATE_ATACKRIGHT:
			xo = (float)((sprite_size + 14) / texture_size) * 2 + ((float)((sprite_size + 8) / texture_size));
			yo = ((float)((sprite_size + 14) / texture_size)) * 3;
			break;

		case STATE_ATACKLEFT:
			xo = ((float)((sprite_size + 8) / texture_size));
			yo = ((float)((sprite_size + 14) / texture_size)) * 3;
			break;
		}

	if(GetState() == STATE_ATACKRIGHT || GetState() == STATE_ATACKLEFT) xf = xo + (float)((sprite_size + 11.0f)/ texture_size);
	else xf = xo + (float)(sprite_size / texture_size);
	if (GetState() == STATE_ATACKDOWN || GetState() == STATE_ATACKUP) yf = yo + (float)((sprite_size + 11.0f) / texture_size);
	else yf = yo + (float)(sprite_size / texture_size);


	DrawRect(tex_id,xo,yf,xf,yo);
}
