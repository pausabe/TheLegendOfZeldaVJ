#pragma once

#include "Globals.h"
#include "cBicho.h"

class cStepladder
{
private:
	int x, y;

public:
	cStepladder();
	~cStepladder();

	void Draw(int tex_id);

	void SetPosition(int x, int y);
	void GetPosition(int *x, int *y);

};
