#pragma once
#include "screen.h"

class Player {
private:
	//プレイヤー情報
	TransForm player;
	float spdX;
	float spdY;

	//重力
	const float grv = 0.3f;

	//メニュー
	Menu* menu;

public:

	Player();
	~Player();

	void Option(char* keys, char* oldkeys);
	void Draw();
};