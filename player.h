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
	int cleanTimer;
	const int cleanTime = 150;
	bool fallFlag;

	int playerGH;

	//スコア
	int score;

	//スクロール
	const int scrollStartX = 640;
	const int scrollStartY = 400;

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
	void Cleaning(char* keys, char* oldkeys);
	void ItemTrash(char* keys, char* oldkeys);
	void PlayerFall();

public:
	//スクロール
	float scrollX;
	float scrollY;

	Player();
	~Player();

	void Tutorial(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Option(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Draw(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);

	void Reset();
};