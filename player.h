#pragma once
#include "screen.h"
#include "item.h"
#include "object.h"

class Player {
public:
	//�v���C���[���
	TransForm player;
	float spdX;
	float spdY;
	int cleanTimer;
	const int cleanTime = 150;
	bool fallFlag;

	int playerGH;

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

public:
	//�X�N���[��
	float scrollX;
	float scrollY;

	Player();
	~Player();

	void Tutorial(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Option(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Draw(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);

	void Reset();
};