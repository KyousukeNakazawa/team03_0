#include <stdlib.h>
#include "DxLib.h"
#include "item.h"
#include "screen.h"
#include "enum.h"

//アイテム同士の当たり判定
bool ItemCollision(float& x, float& y, float& x2, float& y2) {
	float ax1 = x;
	float ay1 = y;
	float ax2 = x + 64;
	float ay2 = y + 96;

	float bx1 = x2;
	float by1 = y2;
	float bx2 = x2 + 64;
	float by2 = y2 + 96;

	return bx1 < ax2&& ax1 < bx2&& by1 < ay2&& ay1 < by2;
}

float Item::SpawnSpaceSearch(float& y, int type) {
	float x;
	bool flag = true;
	int spawnX = WOR_WIDTH - 350;
	x = GetRand(spawnX) + 230;

	//x軸がテーブルが有るところなら確率でテーブルの上に湧く
	if (x >= 1000 && x <= 1800 - 64) {
		int a = GetRand(5);
		if (a >= 3 && type <= WAGOMU) y = 400 - gomi1SizeY;
		else if (a >= 3 && type >= ENPITU) y = 400 - gomi2SizeY;
	}

	//椅子があるところなら確率で椅子の上に湧く
	else if (x >= 1900 && x <= 2200 - 64) {
		int a = GetRand(5);
		if (a >= 3 && type <= WAGOMU) y = 800 - gomi1SizeY;
		else if (a >= 3 && type >= ENPITU) y = 800 - gomi2SizeY;
	}

	//湧き場所にすでにアイテムがあった場合エラー(－1)を返す
	for (int i = 0; i < 20; i++) {
		if (gomi1Type[i] != NONE && ItemCollision(x, y, gomi1X[i], gomi1Y[i])) {
			flag = false;
			break;
		}
		if (gomi2Type[i] != NONE && ItemCollision(x, y, gomi2X[i], gomi2Y[i])) {
			flag = false;
			break;
		}
		if (tool2Type[i] != NONE && ItemCollision(x, y, tool2Y[i], tool2Y[i])) {
			flag = false;
			break;
		}
	}
	if (!flag) return -1;
	return x;
}

Item::Item() {
	spawnType = 0;
	spawnFlag = false;
	spawnTimer = 0;
	for (int i = 0; i < gomi1Num; i++) {
		gomi1Type[i] = NONE;
		cleanFlag[i] = false;
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

	blendNum = 255;

	LoadDivGraph("resource/pict/gomi/gomi1.png", 8, 4, 4, 64, 64, gomi1GH);
	LoadDivGraph("resource/pict/gomi/gomi2.png", 3, 3, 1, 32, 96, gomi2GH);
	LoadDivGraph("resource/pict/tool/tool1.png", 2, 2, 1, 32, 96, tool2GH);
}

Item::~Item() {
}

void Item::Reset() {
	spawnType = 0;
	spawnFlag = false;
	spawnTimer = 0;
	for (int i = 0; i < gomi1Num; i++) {
		gomi1Type[i] = NONE;
		cleanFlag[i] = false;
	}

	for (int i = 0; i < gomi2Num; i++) {
		gomi2Type[i] = NONE;
	}

	for (int i = 0; i < tool2Num; i++) {
		tool2Type[i] = NONE;
		tool2X[i] = 0;
		tool2Y[i] = 0;
	}

	blendNum = 255;
}

void Item::Spawn() {
	//初期配置
	for (int i = 1; i <= 8; i++) {
		while (gomi1Type[i - 1] != i) {
			gomi1Y[i - 1] = WOR_HEIGHT - gomi1SizeY;
			//乱数でスポーン場所を決め重なっていなければスポーン
			gomi1X[i - 1] = SpawnSpaceSearch(gomi1Y[i - 1], i);
			if (gomi1X[i - 1] != -1) {
				gomi1Type[i - 1] = i;
			}
		}
	}
	for (int i = 0; i <= 2; i++) {
		while (gomi2Type[i] != i + 9) {
			gomi2Y[i] = WOR_HEIGHT - gomi2SizeY;
			//乱数でスポーン場所を決め重なっていなければスポーン
			gomi2X[i] = SpawnSpaceSearch(gomi2Y[i], i + 9);
			if (gomi2X[i] != -1) {
				gomi2Type[i] = i + 9;
			}
		}
	}
}

void Item::Tutorial(int scene) {
	//プレイヤーの動きに合わせて変化
	switch (scene) {
	case 0:
		for (int i = 0; i < 3; i++) {
			gomi1Type[i] = 3 + i * 4;
			gomi1X[i] = 300 + 100 * i;
			gomi1Y[i] = WOR_HEIGHT - gomi1SizeY;
		}
		gomi1Type[2] = WAGOMU;
		break;
	default:
		for (int i = 0; i < 7; i++) {
			gomi1Type[i] = i + 1;
			gomi1X[i] = 400 + 100 * i;
			gomi1Y[i] = WOR_HEIGHT - gomi1SizeY;
		}
		gomi1Type[3] = HOKORI;
		gomi1Type[4] = NUNO;
		gomi1Type[5] = WAGOMU;
		gomi1Type[6] = NUNO;

		gomi2Type[0] = TUMAYOUZI;
		gomi2X[0] = 300;
		gomi2Y[0] = WOR_HEIGHT - gomi2SizeY;
		gomi2Type[1] = ENPITU;
		gomi2X[1] = 350;
		gomi2Y[1] = WOR_HEIGHT - gomi2SizeY;
		break;
	}
}

void Item::GomiUpdate() {
	int count = 0;
	//ゴミのスポーン
	if (spawnTimer < spawnTime)spawnTimer++;
	if (spawnTimer >= spawnTime) spawnFlag = true;

	if (spawnFlag) {
		spawnType = GetRand(11) + 1;
		if (spawnType >= ABURA && spawnType <= WAGOMU) {
			for (int i = 0; i < gomi1Num - 6; i++) {
				if (gomi1Type[i] == NONE) {
					gomi1Y[i] = WOR_HEIGHT - gomi1SizeY;
					//乱数でスポーン場所を決め重なっていなければスポーン
					gomi1X[i] = SpawnSpaceSearch(gomi1Y[i], spawnType);
					if (gomi1X[i] != -1) {
						gomi1Type[i] = spawnType;
						spawnTimer = 0;
						spawnFlag = false;
						count = 0;
						break;
					}
				}
				count++;
				if (count >= gomi1Num - 6) {
					spawnTimer = 0;
					spawnFlag = false;
					count = 0;
				}
			}
		}
		else if (spawnType >= ENPITU && spawnType <= TUMAYOUZI) {
			for (int i = 0; i < gomi2Num - 6; i++) {
				if (gomi2Type[i] == NONE) {
					gomi2Y[i] = WOR_HEIGHT - gomi2SizeY;
					gomi2X[i] = SpawnSpaceSearch(gomi2Y[i], spawnType);
					if (gomi2X[i] != -1) {
						gomi2Type[i] = spawnType;
						spawnTimer = 0;
						spawnFlag = false;
						count = 0;
						break;
					}
				}
				count++;
				if (count >= gomi2Num - 6) {
					spawnTimer = 0;
					spawnFlag = false;
					count = 0;
				}
			}
		}
	}
}

void Item::GomiDraw(float scrollX, float scrollY) {
	//絵格納用
	int GH1[20] = {};	//普通のゴミ
	int GH2[20] = {};	//細長いゴミ
	int GH3[10] = {};	//細長い道具

	//普通のゴミ
	for (int i = 0; i < gomi1Num; i++) {
		if (gomi1Type[i] == NONE) GH1[i] = 0;
		for (int j = 0; j < 8; j++) {
			if (gomi1Type[i] == j + 1) GH1[i] = gomi1GH[j];
		}

		if (cleanFlag[i]) SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendNum);
		DrawGraph(gomi1X[i] - scrollX, gomi1Y[i] - scrollY, GH1[i], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//細長いゴミ
	for (int i = 0; i < gomi2Num; i++) {
		if (gomi2Type[i] == NONE) GH2[i] = 0;
		for (int j = 0; j < 3; j++) {
			if (gomi2Type[i] == j + 9) GH2[i] = gomi2GH[j];
		}

		DrawGraph(gomi2X[i] - scrollX, gomi2Y[i] - scrollY, GH2[i], true);
	}

	//細長い道具
	for (int i = 0; i < tool2Num; i++) {
		if (tool2Type[i] == NONE) GH3[i] = 0;
		for (int j = 0; j < 2; j++) {
			if (tool2Type[i] == j + 12) GH3[i] = tool2GH[j];
		}
		DrawGraph(tool2X[i] - scrollX, tool2Y[i] - scrollY, GH3[i], true);
	}

	/*DrawFormatString(0, 0, 0xffffff, "%d", gomi1Type[0]);
	DrawFormatString(0, 15, 0xffffff, "%d", ABURA);*/
	//DrawFormatString(500, 0, 0xffffff, "%d", spawnTimer);
}

