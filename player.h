#pragma once
#include "screen.h"
#include "item.h"
#include "object.h"

class Player {
public:
	//プレイヤー情報
	TransForm player;
	float spdX;
	float spdY;

	int playerGH;

	//スコア
	int score;

	//スクロール
	const int scrollStartX = 640;

	//重力
	const float grv = 0.3f;

	//メニュー
	HUD* menu;

	//アイテム
	Item* item;

	//オブジェクト
	Obj* object;

	void ItemGet(char* keys, char* oldkeys);
	void ItemDrop(char* keys, char* oldkeys);
	void ItemCraft(char* keys, char* oldkeys);
	void ItemTrash(char* keys, char* oldkeys);

public:
	//スクロール
	float scrollX;
	float scrollY;

	Player();
	~Player();

	void Option(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Draw(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
};