#pragma once
#include "screen.h"
#include "item.h"

class Player {
private:
	//�v���C���[���
	TransForm player;
	float spdX;
	float spdY;

	int playerGH;

	//�d��
	const float grv = 0.3f;

	//���j���[
	HUD* menu;

	//�A�C�e��
	Item* item;

	//�w�i
	SCREEN* screen;

	void ItemGet(char* keys, char* oldkeys);
	void ItemDrop(char* keys, char* oldkeys);

public:

	Player();
	~Player();

	void Option(char* keys, char* oldkeys, int& mouse, int& mouseX, int& mouseY);
	void Draw(char* keys, char* oldkeys, int& mouse, int& mouseX, int& mouseY);
};