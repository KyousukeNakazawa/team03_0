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

bool ObjLegsDecision(TransForm& player, TransForm& obj) {
	float ax1 = player.x;
	float ay1 = player.y;
	float ax2 = player.x + player.sizeX;
	float ay2 = player.y + player.sizeY;

	//左脚
	float bx1 = obj.x;
	float by1 = obj.y;
	float bx2 = obj.x + 96;
	float by2 = obj.y + obj.sizeY;

	//右脚
	float cx1 = obj.x + obj.sizeX - 96;
	float cy1 = obj.y;
	float cx2 = obj.x + obj.sizeX;
	float cy2 = obj.y + obj.sizeY;

	return bx1 <= ax2 && ax1 <= bx2 && by1 <= ay2 && ay1 <= by2 ||
		cx1 <= ax2 && ax1 <= cx2 && cy1 <= ay2 && ay1 <= cy2;
}

void ObjDropBlock(TransForm& player, TransForm& obj) {
	if (player.x <= obj.x) player.x = obj.x;
	else if (player.x >= obj.x + obj.sizeX - player.sizeX) player.x = obj.x + obj.sizeX - player.sizeX;
}

Player::Player() {
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 150;
	player.y = WOR_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 7;
	fallFlag = false;
	cleanTimer = 0;
	menu = new HUD;
	item = new Item;
	object = new Obj;

	scrollX = 0;
	scrollY = 600;

	score = 0;

	playerGH = LoadGraph("resource/pict/player.png");
}

Player::~Player() {
	if (menu != NULL) delete menu;
	if (item != NULL) delete item;
	if (object != NULL) delete object;
}

void Player::Reset() {
	menu->Reset();
	item->Reset();
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 150;
	player.y = WOR_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 5;
	fallFlag = false;
	cleanTimer = 0;

	scrollX = 0;
	scrollY = 0;

	score = 0;
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

	//左シフトを押した時地面の上かオブジェクトの上でなら落とせる
	for (int i = 0; i < menu->inventNum; i++) {
		if (menu->onHandFlag[i] && keys[KEY_INPUT_LSHIFT] && !oldkeys[KEY_INPUT_LSHIFT] &&
			(player.y == WOR_HEIGHT - player.sizeY || player.y == object->table.y - player.sizeY + 1 || player.y == object->chair.y - player.sizeY + 1)) {

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

//ゴミをきれいにする
void Player::Cleaning(char* keys, char* oldkeys) {
	for (int i = 0; i < item->gomi1Num; i++) {
		for (int j = 0; j < menu->inventNum; j++) {
			//ゴミの上で特定のアイテムを持っていればきれいにできる
			if (PlayerItemCollision(player, item->gomi1X[i], item->gomi1Y[i], item->gomi1SizeX, item->gomi1SizeY) &&
				menu->onHandFlag[j] && keys[KEY_INPUT_SPACE] && oldkeys[KEY_INPUT_SPACE]) {
				//食べかすの上でほうき
				if (item->gomi1Type[i] == TABEKASU && menu->itemType[j] == HOUKI) {
					cleanTimer++;
					item->blendNum--;
					item->cleanFlag[i] = true;
					//一定時間spaceを押し続けたらゴミが消えスコアゲット
					if (cleanTimer >= cleanTime) {
						score += 2;
						item->gomi1Type[i] = NONE;
						item->gomi1X[i] = -500;
						cleanTimer = 0;
						break;
					}
				}
				//油の上でモップ
				else if (item->gomi1Type[i] == ABURA && menu->itemType[j] == MOP) {
					cleanTimer++;
					item->blendNum--;
					item->cleanFlag[i] = true;
					//一定時間spaceを押し続けたらゴミが消えスコアゲット
					if (cleanTimer >= cleanTime) {
						score += 2;
						item->gomi1Type[i] = NONE;
						item->gomi1X[i] = -500;
						cleanTimer = 0;
						break;
					}
				}
			}
			//一度離したらリセット
			else if (!keys[KEY_INPUT_SPACE] && oldkeys[KEY_INPUT_SPACE]) {
				cleanTimer = 0;
				item->blendNum = 255;
				item->cleanFlag[i] = false;
			}
		}
	}
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

//プレイヤーの落下処理
void Player::PlayerFall() {
	float grv = 0;

	//地面の上かオブジェクトの上以外の時落下
	if (player.y == WOR_HEIGHT - player.sizeY) fallFlag = false;
	else if (player.x + player.sizeX >= object->table.x && player.x <= object->table.x + object->table.sizeX && player.y == object->table.y - player.sizeY + 1) fallFlag = false;
	else if (player.x + player.sizeX >= object->chair.x && player.x <= object->chair.x + object->chair.sizeX && player.y == object->chair.y - player.sizeY + 1) fallFlag = false;
	else fallFlag = true;

	if (fallFlag) {
		grv = 2;
		player.y += grv;
	}
	else {
		grv = 0;
	}
}

void Player::Tutorial(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	PlayerFall();

	//オブジェクトの脚に触れていたら登れる
	if (ObjLegsDecision(player, object->table) || ObjLegsDecision(player, object->chair)) {
		player.y -= ((float)(keys[KEY_INPUT_W]) - (float)(keys[KEY_INPUT_S])) * spdY;
		fallFlag = false;
	}

	//画面外に出ない
	if (player.x <= 0) player.x = 0;
	else if (player.x >= WOR_WIDTH - player.sizeX) player.x = WOR_WIDTH - player.sizeX;


	if (player.y >= WOR_HEIGHT - player.sizeY) player.y = WOR_HEIGHT - player.sizeY;

	//オブジェクトの上で止まる
	//机
	if (player.x + player.sizeX >= object->table.x && player.x <= object->table.x + object->table.sizeX) {
		if (player.y <= object->table.y - player.sizeY) player.y = object->table.y - player.sizeY + 1;
	}

	//椅子
	else if (player.x + player.sizeX >= object->chair.x && player.x <= object->chair.x + object->chair.sizeX) {
		if (!fallFlag && player.y <= object->chair.y - player.sizeY) player.y = object->chair.y - player.sizeY + 1;
	}


	//スクロール
	scrollX = ScrollX(player.x, scrollStartX);
	scrollY = ScrollY(player.y, scrollStartY);

	//アイテムの取得
	ItemGet(keys, oldkeys);

	//アイテム落とす
	ItemDrop(keys, oldkeys);

	Cleaning(keys, oldkeys);

	//アイテムをゴミ箱に捨てる
	ItemTrash(keys, oldkeys);
}

void Player::Option(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	PlayerFall();

	//オブジェクトの脚に触れていたら登れる
	if (ObjLegsDecision(player, object->table) || ObjLegsDecision(player, object->chair)) {
		player.y -= ((float)(keys[KEY_INPUT_W]) - (float)(keys[KEY_INPUT_S])) * spdY;
		fallFlag = false;
	}

	//画面外に出ない
	if (player.x <= 0) player.x = 0;
	else if (player.x >= WOR_WIDTH - player.sizeX) player.x = WOR_WIDTH - player.sizeX;


	if (player.y >= WOR_HEIGHT - player.sizeY) player.y = WOR_HEIGHT - player.sizeY;

	//オブジェクトの上で止まる
	//机
	if (player.x + player.sizeX >= object->table.x && player.x <= object->table.x + object->table.sizeX) {
		if (player.y <= object->table.y - player.sizeY) player.y = object->table.y - player.sizeY + 1;
	}

	//椅子
	else if (player.x + player.sizeX >= object->chair.x && player.x <= object->chair.x + object->chair.sizeX) {
		if (!fallFlag && player.y <= object->chair.y - player.sizeY) player.y = object->chair.y - player.sizeY + 1;
	}


	//スクロール
	scrollX = ScrollX(player.x, scrollStartX);
	scrollY = ScrollY(player.y, scrollStartY);

	//アイテムの湧き
	item->GomiUpdate();

	//アイテムの取得
	ItemGet(keys, oldkeys);

	//アイテム落とす
	ItemDrop(keys, oldkeys);

	Cleaning(keys, oldkeys);

	//アイテムをゴミ箱に捨てる
	ItemTrash(keys, oldkeys);
}

void Player::Draw(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY) {
	//キッチン
	object->KitchenDraw(scrollX, scrollY);

	//テーブル
	object->TableDraw(scrollX, scrollY);

	//椅子
	object->ChairDraw(scrollX, scrollY);

	//ゴミ箱
	object->TrashBoxDraw(scrollX, scrollY);

	//プレイヤー
	DrawGraph(player.x - scrollX, player.y - scrollY, playerGH, true);

	//ゴミ
	item->GomiDraw(scrollX, scrollY);

	//HUD
	menu->Craft(keys, oldkeys, mouse, oldMouse, mouseX, mouseY);
	menu->Inventory(mouse, mouseX, mouseY);

	DrawFormatString(500, 0, 0xffffff, "%d", fallFlag);
	DrawFormatString(530, 0, 0xffffff, "%f", player.y);
}