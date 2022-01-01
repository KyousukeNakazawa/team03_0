#pragma once
#include "screen.h"

class Enemy
{
	Object human;
	Object rumba;
	Object cat;
	Object dog;

	Enemy();

	void HumanMove();
	void HumanDraw(int scroll);

	void RumbaMove();
	void RumbaDraw(int scroll);

	void CatMove();
	void CatDraw(int scroll);

	void DogMove();
	void DogDraw(int scroll);
};