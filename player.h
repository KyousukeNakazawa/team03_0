#pragma once
#include "screen.h"

class Player {
private:
	//�v���C���[���
	TransForm player;
	float spdX;
	float spdY;

	//�d��
	const float grv = 0.3f;

	//���j���[
	Menu* menu;

public:

	Player();
	~Player();

	void Option(char* keys, char* oldkeys);
	void Draw();
};