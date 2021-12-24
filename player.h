#pragma once
#include "screen.h"
#include "item.h"

class Player {
private:
	//プレイヤー情報
	TransForm player;
	float spdX;
	float spdY;

	int playerGH;

	//重力
	const float grv = 0.3f;

	//メニュー
	HUD* menu;

	//アイテム
	Item* item;

	//背景
	SCREEN* screen;

	void ItemGet(char* keys, char* oldkeys);
	void ItemDrop(char* keys, char* oldkeys);

public:

	Player();
	~Player();

	void Option(char* keys, char* oldkeys, int& mouse, int& mouseX, int& mouseY);
	void Draw(char* keys, char* oldkeys, int& mouse, int& mouseX, int& mouseY);
};