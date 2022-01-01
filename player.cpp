#include "DxLib.h"
#include "player.h"
#include"screen.h"
#include"enum.h"

//プレイヤーとアイテムの当たり判定
bool PlayerItemCollision(TransForm& player, float& itemX, float& itemY, float sizeX, float sizeY) {
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

bool TrashBoxCollision(TransForm& player, TransForm& box) {
	float ax1 = player.x;
	float ay1 = player.y;
	float ax2 = player.x + player.sizeX;
	float ay2 = player.y + player.sizeY;

	float bx1 = box.x;
	float by1 = box.y;
	float bx2 = box.x + box.sizeX;
	float by2 = box.y + box.sizeY;

	return bx1 < ax2&& ax1 < bx2&& by1 < ay2&& ay1 < by2;
}

Player::Player() {
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 150;
	player.y = WIN_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 0;
	menu = new HUD;
	item = new Item;
	object = new Obj;

	scrollX = 0;
	scrollY = 0;

	score = 0;

	playerGH = LoadGraph("resource/pict/player.png");
}

Player::~Player() {
	if (menu != NULL) delete menu;
	if (item != NULL) delete item;
	if (object != NULL) delete object;
}

void Player::ItemGet(char* keys, char* oldkeys) {
	//アイテムの取得
	//普通のゴミ
	for (int i = 0; i < item->gomi1Num; i++) {
		if (PlayerItemCollision(player, item->gomi1X[i], item->gomi1Y[i], item->gomi1SizeX, item->gomi1SizeY) &&
			keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE] && item->gomi1Type[i] >= HOKORI) {
			for (int j = 0; j < menu->inventNum; j++) {
				if (menu->itemType[j] == NONE) {
					menu->itemType[j] = item->gomi1Type[i];
					item->gomi1Type[i] = NONE;
					item->gomi1X[i] = -500;
				}
			}
		}
	}

	//細長いゴミ
	for (int i = 0; i < item->gomi2Num; i++) {
		if (PlayerItemCollision(player, item->gomi2X[i], item->gomi2Y[i], item->gomi2SizeX, item->gomi2SizeY) &&
			keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
			for (int j = 0; j < menu->inventNum; j++) {
				if (menu->itemType[j] == NONE) {
					menu->itemType[j] = item->gomi2Type[i];
					item->gomi2Type[i] = NONE;
					item->gomi2X[i] = -500;
				}
			}
		}
	}

	//道具
	for (int i = 0; i < item->tool2Num; i++) {
		if (PlayerItemCollision(player, item->tool2X[i], item->tool2Y[i], item->tool2SizeX, item->tool2SizeY) &&
			keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
			for (int j = 0; j < menu->inventNum; j++) {
				if (menu->itemType[j] == NONE) {
					menu->itemType[j] = item->tool2Type[i];
					item->tool2Type[i] = NONE;
					item->tool2X[i] = -500;
				}
			}
		}
	}
}

//アイテムを落とす処理
void Player::ItemDrop(char* keys, char* oldkeys) {
	bool dropFlag = true;

	//落とした時アイテム同士が重なる場合落とせない
	for (int i = 0; i < item->gomi1Num; i++) {
		if (PlayerItemCollision(player, item->gomi1X[i], item->gomi1Y[i], item->gomi1SizeX, item->gomi1SizeY)) {
			dropFlag = false;
			break;
		}
	}
	for (int j = 0; j < item->gomi2Num; j++) {
		if (PlayerItemCollision(player, item->gomi2X[j], item->gomi2Y[j], item->gomi2SizeX, item->gomi2SizeY)) {
			dropFlag = false;
			break;
		}
	}
	for (int j = 0; j < item->tool2Num; j++) {
		if (PlayerItemCollision(player, item->tool2X[j], item->tool2Y[j], item->tool2SizeX, item->tool2SizeY)) {
			dropFlag = false;
			break;
		}
	}

	//左シフトを押すと落とせる
	for (int i = 0; i < menu->inventNum; i++) {
		if (menu->onHandFlag[i] && keys[KEY_INPUT_LSHIFT] && !oldkeys[KEY_INPUT_LSHIFT]) {


			//普通のゴミ
			for (int j = 0; j < item->gomi1Num; j++) {
				if (menu->itemType[i] >= ABURA && menu->itemType[i] <= WAGOMU && item->gomi1Type[j] == NONE && dropFlag) {

					item->gomi1Type[j] = menu->itemType[i];
					item->gomi1X[j] = player.x;
					item->gomi1Y[j] = player.y + player.sizeY - item->gomi1SizeY;
					menu->itemType[i] = NONE;
					break;
				}
			}

			//細長いゴミ
			for (int j = 0; j < item->gomi2Num; j++) {
				if (menu->itemType[i] >= ENPITU && menu->itemType[i] <= TUMAYOUZI && item->gomi2Type[j] == NONE && dropFlag) {
					item->gomi2Type[j] = menu->itemType[i];
					item->gomi2X[j] = player.x;
					item->gomi2Y[j] = player.y + player.sizeY - item->gomi2SizeY;
					menu->itemType[i] = NONE;
					break;
				}
			}

			//道具
			for (int j = 0; j < item->tool2Num; j++) {
				if (menu->itemType[i] >= HOUKI && menu->itemType[i] <= MOP && item->tool2Type[j] == NONE && dropFlag) {
					item->tool2Type[j] = menu->itemType[i];
					item->tool2X[j] = player.x;
					item->tool2Y[j] = player.y + player.sizeY - item->tool2SizeY;
					menu->itemType[i] = NONE;
					break;
				}
			}
		}
	}
}

//アイテムをクラフト
void Player::ItemCraft(char* keys, char* oldkeys) {

}

//アイテムをゴミ箱に捨てスコアゲット
void Player::ItemTrash(char* keys, char* oldkeys) {
	for (int i = 0; i < menu->inventNum; i++) {
		if (menu->onHandFlag[i] && keys[KEY_INPUT_F] && !oldkeys[KEY_INPUT_F] &&
			TrashBoxCollision(player, object->trashBox) && menu->itemType[i] >= HOKORI && menu->itemType[i] <= TUMAYOUZI) {
			score++;
			menu->itemType[i] = NONE;
		}
	}
}

void Player::Option(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	scrollX = Scroll(player.x, spdX, scrollStartX);

	//アイテムの湧き
	item->GomiUpdate();

	//アイテムの取得
	ItemGet(keys, oldkeys);

	//アイテム落とす
	ItemDrop(keys, oldkeys);

	//アイテムをゴミ箱に捨てる
	ItemTrash(keys, oldkeys);
}

void Player::Draw(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY) {
	//HUD
	menu->Craft(keys, oldkeys, mouse, oldMouse, mouseX, mouseY);
	menu->Inventory(mouse, mouseX, mouseY);

	//ゴミ箱
	object->TrashBoxDraw(scrollX);

	//プレイヤー
	DrawGraph(player.x - scrollX, player.y, playerGH, true);

	//ゴミ
	item->GomiDraw(scrollX);
}