#pragma once
#include "screen.h"
#include "item.h"
#include "object.h"
#include "enemy.h"


class Player {
public:
	//プレイヤー情報
	TransForm player;
	float spdX;
	float spdY;
	bool fallFlag;
	bool hideFlag;

	int spawnType;
	bool spawnFlag;

	AniGHTimer playerMove = { 4, 40, 0 };
	int playerRightGH[4];
	int playerLeftGH[4];
	int playerFallRightGH[4];
	int playerFallLeftGH[4];
	int playerClimbGH[4];
	int playerHoukiGH[4];
	int playerMopGH[4];
	int playerWaitGH[4];
	//int playerGH;

	void PlayerAniGH(char* keys, char* oldkeys);
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

	//エネミー
	Enemy* enemy;
	void RoombaCollision(Object rumba);

	//音
	int walkSE;
	int houkiSE;
	int mopSE;
	int trashPointSE;
	int cleanPointSE;
	int climbSE;



public:
	//スクロール
	float scrollX;
	float scrollY;

	Player();
	~Player();

	void Tutorial(char* keys, char* oldkeys, int& tutorialScene);
	void Option(char* keys, char* oldkeys, int& scene);
	void Draw(char* keys, char* oldkeys);
	void Sound(char* keys, char* oldkeys);
	void SoundStop();
	void Reset(int scene);
};