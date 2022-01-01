#pragma once
#include "screen.h"


class Item {
private:

	int spawnTimer;
	const int spawnTime = 100;

public:
	const int gomi1Num = 15;
	const int gomi2Num = 15;
	const int tool2Num = 10;

	float gomi1X[15];
	float gomi1Y[15];
	float gomi1SizeX;
	float gomi1SizeY;
	int gomi1Type[15];

	float gomi2X[15];
	float gomi2Y[15];
	float gomi2SizeX;
	float gomi2SizeY;
	int gomi2Type[15];

	float tool2X[10];
	float tool2Y[10];
	float tool2SizeX;
	float tool2SizeY;
	int tool2Type[10];

	int gomi1GH[8];
	int gomi2GH[3];

	int tool2GH[2];

	Item();
	~Item();

	void GomiUpdate();
	void GomiDraw(float scrollX);

	void TrashBoxDate();
};