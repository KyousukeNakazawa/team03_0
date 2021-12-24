#include <stdlib.h>
#include "DxLib.h"
#include "item.h"
#include "screen.h"
#include "enum.h"

Item::Item() {
	spawnTimer = 0;
	for (int i = 0; i < gomi1Num; i++) {
		gomi1Type[i] = NONE;
	}

	for (int i = 0; i < gomi2Num; i++) {
		gomi2Type[i] = NONE;
	}

	gomi1SizeX = 64;
	gomi1SizeY = 64;
	gomi2SizeX = 32;
	gomi2SizeY = 96;

	//screen = new SCREEN;

	LoadDivGraph("resource/pict/gomi/gomi1.png", 8, 4, 4, 64, 64, gomi1GH);

	//‰Šú”z’u
	for (int i = 1; i <= 8; i++) {
		gomi1Type[i - 1] = i;
		gomi1X[i - 1] = 230 + (70 * i);
		gomi1Y[i - 1] = WIN_HEIGHT - gomi1SizeY;
	}
	for (int i = 1; i <= 3; i++) {
		gomi2Type[i - 1] = i + 8;
	}

}

Item::~Item() {
	//if (screen != NULL) delete screen;
}

void Item::GomiUpdate() {
	//spawnTimer++;
	if (spawnTimer >= spawnTime) {
		spawnTimer = 0;
		for (int i = 0; i < gomi1Num; i++) {
			if (gomi1Type[i] == NONE) {
				gomi1Type[i] = CAN;
				gomi1X[i] = 500;
				gomi1Y[i] = WIN_HEIGHT - gomi1SizeY;
				break;
			}
		}
	}
}

void Item::GomiDraw(float scrollX) {
	int GH1[15] = {};

	for (int i = 0; i < gomi1Num; i++) {
		if (gomi1Type[i] == NONE) GH1[i] = 0;
		for (int j = 0; j < 8; j++) {
			if (gomi1Type[i] == j + 1) GH1[i] = gomi1GH[j];
		}

		DrawGraph(gomi1X[i] - scrollX , gomi1Y[i], GH1[i], true);
	}
	DrawFormatString(0, 0, 0xffffff, "%d", gomi1Type[0]);
	DrawFormatString(0, 15, 0xffffff, "%d", ABURA);
}