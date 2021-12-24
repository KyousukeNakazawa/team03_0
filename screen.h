#pragma once
#include "item.h"

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 800;

const int WOR_WIDTH = 2560;

//オブジェクト基本情報
struct TransForm {
	float x;
	float y;
	float sizeX;
	float sizeY;
};

int Scroll(float& playerPos, float& speed, const float scrollStart);

class SCREEN {
private:
	float backX;
	float backY;
	float scrollX;
	float scrollY;

	int backGH;

public:
	const int scrollStartX = 640;

	SCREEN();
	~SCREEN();
	void SetScrollX(float scrollX) { this->scrollX = scrollX; }
	float GetScrollX() { return scrollX; }

	void Background();
};

class HUD {
private:
	//クラフトメニュー
	TransForm craft;
	bool craftFlag;

	//ホットバー
	const int minInvent = 4;
	const int maxInvent = 6;
	TransForm invent;

	//アイテム
	TransForm itemGH;

	int inventGH[2];
	int gomi1GH[8];

public:
	int inventNum;

	bool onHandFlag[6];
	int itemType[6];

	HUD();
	~HUD();
	void Craft(char* keys, char* oldkeys);
	void Inventory(int& mouse, int& mouseX, int& mouseY);

};