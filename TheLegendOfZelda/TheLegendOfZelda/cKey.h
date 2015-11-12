#pragma once

#include "cObject.h"

class cKey : public cObject
{
private:
	int keyId;
public:
	cKey();
	~cKey();

	void Draw(int tex_id);
	void SetKeyId(int id);
	int GetKeyId();
};
