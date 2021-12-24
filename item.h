#pragma once
#include "screen.h"

class Item {
private:

	int spawnTimer;
	const int spawnTime = 100;

public:
	const int gomi1Num = 15;
	const int gomi2Num = 5;

	float gomi1X[15];
	float gomi1Y[15];
	float gomi1SizeX;
	float gomi1SizeY;
	int gomi1Type[15];

	float gomi2X[5];
	float gomi2Y[5];
	float gomi2SizeX;
	float gomi2SizeY;
	int gomi2Type[5];

	int gomi1GH[8];

	Item();
	~Item();

	void GomiUpdate();
	void GomiDraw(float scrollX);
};