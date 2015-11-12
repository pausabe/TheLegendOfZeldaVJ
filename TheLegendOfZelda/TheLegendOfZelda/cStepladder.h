#pragma once

#include "cObject.h"

class cStepladder : public cObject
{
private:

public:
	cStepladder();
	~cStepladder();

	void Draw(int tex_id);
	int GetKeyId();

};
