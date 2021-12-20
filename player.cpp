#include "DxLib.h"
#include "player.h"
#include"screen.h"

Player::Player() {
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 36;
	player.y = WIN_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 0;
	menu = new class Menu();
}

Player::~Player() {
	if (menu != NULL) {
		delete menu;
	}
}

void Player::Option(char* keys, char* oldkeys) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	menu->Craft(keys, oldkeys);
}

void Player::Draw() {
	//DrawGraph();
	DrawBox(player.x, player.y, player.x + player.sizeX, player.y + player.sizeY, 0xffffff, true);
}