#include "DxLib.h"
#include "screen.h"
#include "enum.h"

int Scroll(float& playerPos, float& speed, const float scrollStart) {
	if (playerPos > WOR_WIDTH - scrollStart) {
		return WIN_WIDTH;
	}
	else if (playerPos < scrollStart) {
		return 0;
	}
	else {
		return playerPos - scrollStart;
	}
}

SCREEN::SCREEN() {
	backX = 0;
	backY = 0;
	scrollX = 0;
	scrollY = 0;

	backGH = LoadGraph("resource/pict/background.png");
}

SCREEN::~SCREEN() {

}
void SCREEN::Background() {
	DrawGraph(backX - scrollX, backY, backGH, true);
}

HUD::HUD() {
	craft.sizeX = 384;
	craft.sizeY = 250;
	craft.x = 20;
	craft.y = 150;
	craftFlag = false;

	invent.x = 20;
	invent.y = 20;
	invent.sizeX = 96;
	invent.sizeY = 96;
	inventNum = minInvent;

	for (int i = 0; i < maxInvent; i++) {
		onHandFlag[i] = false;
		itemType[i] = NONE;
	}

	LoadDivGraph("resource/pict/inventory.png", 2, 2, 1, 96, 96, inventGH);
	LoadDivGraph("resource/pict/gomi/gomi1.png", 8, 4, 4, 64, 64, gomi1GH);
}

HUD::~HUD() {

}

void HUD::Craft(char* keys, char* oldkeys) {
	//クラフトメニューの表示
	if (!oldkeys[KEY_INPUT_R] && keys[KEY_INPUT_R]) {
		if (!craftFlag) {
			craftFlag = true;
		}
		else {
			craftFlag = false;
		}
	}
	if (craftFlag) {
		//DrawGraph();
		DrawBox(craft.x, craft.y, craft.x + craft.sizeX, craft.y + craft.sizeY, GetColor(230, 230, 230), true);
	}
}

void HUD::Inventory(int& mouse, int& mouseX, int& mouseY) {
	int invGH[6] = {};
	int itemGH[8] = {};

	//ホットバーの表示 クリックで持つアイテムを選択
	for (int i = 0; i < inventNum; i++) {
		invGH[i] = inventGH[off_hand];
		if (mouseY > invent.y && mouseY < invent.y + invent.sizeY &&
			mouseX > invent.x + invent.sizeX * i && mouseX < invent.x + invent.sizeX * (i + 1)) {
			if ((mouse & MOUSE_INPUT_LEFT) && !onHandFlag[i]) {
				for (int j = 0; j < inventNum; j++) {
					onHandFlag[j] = false;
				}
				onHandFlag[i] = true;
			}
		}

		if (onHandFlag[i]) {
			invGH[i] = inventGH[on_hand];
		}
		else {
			invGH[i] = inventGH[off_hand];
		}

		if (itemType[i] == NONE) itemGH[i] = 0;
		for (int j = 0; j < 8; j++) {
			if (itemType[i] == j + 1) itemGH[i] = gomi1GH[j];
		}

		DrawGraph(invent.x + invent.sizeX * i, invent.y, invGH[i], true);
		DrawExtendGraph(invent.x + invent.sizeX * i, invent.y, invent.x + invent.sizeX * (i + 1), invent.y + invent.sizeY, itemGH[i], true);
	}
}