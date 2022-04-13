#pragma once
#include "screen.h"
#include "object.h"

struct Vec2
{
	float x;
	float y;
};


class Enemy
{
public:
	Object human;
	bool humanFlag;
	Object roomba;
	bool roombaFlag;

	Obj* object;

	Vec2 a;
	Vec2 b;
	Vec2 c;
	Vec2 points[3];
	float size;
	bool hitFlag;

	//当たり判定のフラグ
	bool left_up_flag;
	bool right_up_flag;
	bool left_down_flag;
	bool right_down_flag;

	//敵のアニメーション
	int humanLeftGH[4];
	int humanRightGH[4];
	int humanNum;
	int roombaGH[2];
	AniGHTimer humanMove = { 4, 48, 0 };

	//敵の向き
	bool humanDirectFlag;
	bool roombaDirectFlag;

	//敵が出てくるタイミングの変数
	int randomNumber;
	int randomTimer;
	int coolTimer;
	bool enemyFlag;

	TransForm keikoku;
	int keikokuGH;

	Enemy();
	~Enemy();

	void EnemyTiming(float scrollX, float scrollY);

	void HumanMove();
	void HumanDraw(float scrollX, float scrollY);

	void RoombaMove();
	void RoombaDraw(float scrollX, float scrollY);

	//見つかる範囲の判定関数
	void HumanSearchCollision(float player_x, float player_y, float player_width, float player_height, float scrollX, float scrollY, bool hideFlag);

	void Warning();

};