#include "cKey.h"

cKey::cKey() {
	w = 7 * (float)TILE_SIZE / 16;
	h = TILE_SIZE;
}
cKey::~cKey() {}

void cKey::Draw(int tex_id)
{
	float xo, yo, xf, yf;

	float sprite_size = 16;
	float texture_size = 256;


	xo = (float)(240 / texture_size);
	yo = 0.0f;
	xf = xo + (float)(7*sprite_size/16 / texture_size);
	yf = yo + (float)(sprite_size / texture_size);


	DrawRect(tex_id, xo, yf, xf, yo);

}

void cKey::SetKeyId(int id) {
	keyId = id;
}

int cKey::GetKeyId() {
	return keyId;
}