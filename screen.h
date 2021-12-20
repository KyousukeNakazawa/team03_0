#pragma once

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 800;

//オブジェクト基本情報
struct TransForm {
	float x;
	float y;
	float sizeX;
	float sizeY;
};

class Menu {
private:
	TransForm craft;
	bool menuFlag;

public:
	Menu();
	~Menu();
	void Craft(char* keys, char* oldkeys);
};