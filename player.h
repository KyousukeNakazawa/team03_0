#pragma once
#include "screen.h"
#include "item.h"
#include "object.h"
#include "enemy.h"


class Player {
public:
	//�v���C���[���
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
	//�X�R�A
	int score;

	//�X�N���[��
	const int scrollStartX = 640;
	const int scrollStartY = 400;

	//�d��
	const float grv = 0.3f;

	//���j���[
	HUD* menu;

	//�A�C�e��
	Item* item;

	//�I�u�W�F�N�g
	Obj* object;

	void ItemGet(char* keys, char* oldkeys);
	void ItemDrop(char* keys, char* oldkeys);
	void Cleaning(char* keys, char* oldkeys);
	void ItemTrash(char* keys, char* oldkeys);
	void PlayerFall();

	//�G�l�~�[
	Enemy* enemy;
	void RoombaCollision(Object rumba);

	//��
	int walkSE;
	int houkiSE;
	int mopSE;
	int trashPointSE;
	int cleanPointSE;
	int climbSE;



public:
	//�X�N���[��
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