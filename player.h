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

	int playerGH;

	//�X�R�A
	int score;

	//�X�N���[��
	const int scrollStartX = 640;

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
	void ItemCraft(char* keys, char* oldkeys);
	void ItemTrash(char* keys, char* oldkeys);

public:
	//�X�N���[��
	float scrollX;
	float scrollY;

	Player();
	~Player();

	void Option(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Draw(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
};