#include "DxLib.h"
#include "screen.h"

Menu::Menu() {
	craft.sizeX = 200;
	craft.sizeY = 250;
	craft.x = 20;
	craft.y = 150;
	menuFlag = false;
}

Menu::~Menu() {
}

void Menu::Craft(char* keys, char* oldkeys) {
	if (!oldkeys[KEY_INPUT_R] && keys[KEY_INPUT_R]) {
		if (!menuFlag) {
			menuFlag = true;
		}
		else {
			menuFlag = false;
		}
	}
	if (menuFlag) {
		//DrawGraph();
		DrawBox(craft.x, craft.y, craft.x + craft.sizeX, craft.y + craft.sizeY, GetColor(230, 230, 230), true);
	}
}