#include <stdlib.h>
#include "DxLib.h"
#include "item.h"
#include "screen.h"
#include "enum.h"

//ƒAƒCƒeƒ€“¯Žm‚Ì“–‚½‚è”»’è
bool ItemCollision(TransForm& item1, TransForm& item2) {
	float ax1 = item1.x;
	float ay1 = item1.y;
	float ax2 = item1.x + item1.sizeX;
	float ay2 = item1.y + item1.sizeY;

	float bx1 = item2.x;
	float by1 = item2.y;
	float bx2 = item2.x + item2.sizeX;
	float by2 = item2.y + item2.sizeY;

	return bx1 < ax2&& ax1 < bx2&& by1 < ay2&& ay1 < by2;
}

Item::Item() {
	spawnTimer = 0;
	for (int i = 0; i < gomi1Num; i++) {
		gomi1Type[i] = NONE;
	}

	for (int i = 0; i < gomi2Num; i++) {
		gomi2Type[i] = NONE;
	}

	for (int i = 0; i < tool2Num; i++) {
		tool2Type[i] = NONE;
		tool2X[i] = 0;
		tool2Y[i] = 0;
	}

	gomi1SizeX = 64;
	gomi1SizeY = 64;
	gomi2SizeX = 32;
	gomi2SizeY = 96;
	tool2SizeX = 32;
	tool2SizeY = 96;

	//screen = new SCREEN;

	LoadDivGraph("resource/pict/gomi/gomi1.png", 8, 4, 4, 64, 64, gomi1GH);
	LoadDivGraph("resource/pict/gomi/gomi2.png", 3, 3, 1, 32, 96, gomi2GH);
	LoadDivGraph("resource/pict/tool/tool1.png", 2, 2, 1, 32, 96, tool2GH);

	//‰Šú”z’u
	//•’Ê‚ÌƒSƒ~
	for (int i = 1; i <= 8; i++) {
		gomi1Type[i - 1] = i;
		gomi1X[i - 1] = 230 + (70 * i);
		gomi1Y[i - 1] = WIN_HEIGHT - gomi1SizeY;
	}
	gomi1Type[8] = HOKORI;
	gomi1X[8] = 1100;
	gomi1Y[8] = WIN_HEIGHT - gomi1SizeY;
	//×’·‚¢ƒSƒ~
	for (int i = 0; i <= 2; i++) {
		gomi2Type[i] = i + 9;
		gomi2X[i] = 860 + (70 * i);
		gomi2Y[i] = WIN_HEIGHT - gomi2SizeY;
	}

}

Item::~Item() {
	//if (screen != NULL) delete screen;
}

void Item::GomiUpdate() {
	//ƒSƒ~‚ÌƒXƒ|[ƒ“
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
	//ŠGŠi”[—p
	int GH1[15] = {};	//•’Ê‚ÌƒSƒ~
	int GH2[15] = {};	//×’·‚¢ƒSƒ~
	int GH3[10] = {};	//×’·‚¢“¹‹ï

	//•’Ê‚ÌƒSƒ~
	for (int i = 0; i < gomi1Num; i++) {
		if (gomi1Type[i] == NONE) GH1[i] = 0;
		for (int j = 0; j < 8; j++) {
			if (gomi1Type[i] == j + 1) GH1[i] = gomi1GH[j];
		}

		DrawGraph(gomi1X[i] - scrollX, gomi1Y[i], GH1[i], true);
	}

	//×’·‚¢ƒSƒ~
	for (int i = 0; i < gomi2Num; i++) {
		if (gomi2Type[i] == NONE) GH2[i] = 0;
		for (int j = 0; j < 3; j++) {
			if (gomi2Type[i] == j + 9) GH2[i] = gomi2GH[j];
		}

		DrawGraph(gomi2X[i] - scrollX, gomi2Y[i], GH2[i], true);
	}

	//×’·‚¢“¹‹ï
	for (int i = 0; i < tool2Num; i++) {
		if (tool2Type[i] == NONE) GH3[i] = 0;
		for (int j = 0; j < 2; j++) {
			if (tool2Type[i] == j + 12) GH3[i] = tool2GH[j];
		}
		DrawGraph(tool2X[i] - scrollX, tool2Y[i], GH3[i], true);
	}

	/*DrawFormatString(0, 0, 0xffffff, "%d", gomi1Type[0]);
	DrawFormatString(0, 15, 0xffffff, "%d", ABURA);*/
}

void Item::TrashBoxDate() {

}