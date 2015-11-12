#pragma once

#include "Globals.h"
#include "cObject.h"

class cKey : public cObject
{
private:

public:
	cKey();
	~cKey();

	void Draw(int tex_id);

};
