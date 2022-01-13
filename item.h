#pragma once
#include "screen.h"


class Item {
private:

	int spawnType;
	bool spawnFlag;
	int spawnTimer;
	const int spawnTime = 200;

	float SpawnSpaceSearch(float& y, int type);

public:
	const int gomi1Num = 20;
	const int gomi2Num = 20;
	const int tool2Num = 10;

	float gomi1X[20];
	float gomi1Y[20];
	float gomi1SizeX;
	float gomi1SizeY;
	int gomi1Type[20];
	bool cleanFlag[20];
	int blendNum;

	float gomi2X[20];
	float gomi2Y[20];
	float gomi2SizeX;
	float gomi2SizeY;
	int gomi2Type[20];

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
	void GomiDraw(float scrollX, float scrollY);


	void Reset();
};