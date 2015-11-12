#pragma once


class cEnemy
{
public:
	cEnemy();
	~cEnemy();

	virtual bool Blockable() = 0;
};
