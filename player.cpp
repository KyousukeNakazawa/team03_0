#include "DxLib.h"
#include "player.h"
#include"screen.h"
#include"enum.h"

//プレイヤーとアイテムの当たり判定
bool ItemCollision(TransForm& player, float& itemX, float& itemY, float sizeX, float sizeY) {
	float ax1 = player.x;
	float ay1 = player.y;
	float ax2 = player.x + player.sizeX;
	float ay2 = player.y + player.sizeY;

	float bx1 = itemX;
	float by1 = itemY;
	float bx2 = itemX + sizeX;
	float by2 = itemY + sizeY;

	return bx1 < ax2&& ax1 < bx2&& by1 < ay2&& ay1 < by2;
}

Player::Player() {
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 36;
	player.y = WIN_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 0;
	menu = new HUD;
	screen = new SCREEN;
	item = new Item;

	playerGH = LoadGraph("resource/pict/player.png");
}

Player::~Player() {
	if (menu != NULL) delete menu;
	if (item != NULL) delete item;
	if (screen != NULL) delete screen;
}

void Player::ItemGet(char* keys, char* oldkeys) {
	//アイテムの取得
	for (int i = 0; i < item->gomi1Num; i++) {
		if (ItemCollision(player, item->gomi1X[i], item->gomi1Y[i], item->gomi1SizeX, item->gomi1SizeY) &&
			keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
			for (int j = 0; j < menu->inventNum; j++) {
				if (menu->itemType[j] == NONE) {
					menu->itemType[j] = item->gomi1Type[i];
					item->gomi1Type[i] = NONE;
				}
			}
		}
	}
}

void Player::ItemDrop(char* keys, char* oldkeys) {
	for (int i = 0; i < menu->inventNum; i++) {
		if (menu->onHandFlag[i] && keys[KEY_INPUT_LSHIFT] && !oldkeys[KEY_INPUT_LSHIFT]) {
			for (int j = 0; j < item->gomi1Num; j++) {
				if (menu->itemType[i] <= 8 && item->gomi1Type[j] == NONE) {

					item->gomi1Type[j] = menu->itemType[i];
					item->gomi1X[j] = player.x;
					item->gomi1Y[j] = player.y + player.sizeY - item->gomi1SizeY;
					menu->itemType[i] = NONE;
					break;
				}
			}
		}
	}
}

void Player::Option(char* keys, char* oldkeys, int& mouse, int& mouseX, int& mouseY) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	screen->SetScrollX(Scroll(player.x, spdX, screen->scrollStartX));

	//アイテムの湧き
	item->GomiUpdate();

	//アイテムの取得
	ItemGet(keys, oldkeys);

	//アイテム落とす
	ItemDrop(keys, oldkeys);
}

void Player::Draw(char* keys, char* oldkeys, int& mouse, int& mouseX, int& mouseY) {
	screen->Background();
	menu->Craft(keys, oldkeys);
	menu->Inventory(mouse, mouseX, mouseY);
	DrawGraph(player.x - screen->GetScrollX(), player.y, playerGH, true);
	item->GomiDraw(screen->GetScrollX());
}