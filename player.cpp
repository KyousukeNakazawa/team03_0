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

	return (bx1 <= ax2 && ax1 <= bx2 && by1 <= ay2 && ay1 <= by2) ||
		(cx1 <= ax2 && ax1 <= cx2 && cy1 <= ay2 && ay1 <= cy2);
}

bool ObjectCollision(TransForm player, TransForm obj) {
	float ax1 = player.x + 6;
	float ax2 = player.x + player.sizeX - 6;

	float bx1 = obj.x;
	float bx2 = obj.x + obj.sizeX;

	return ax1 >= bx1 && ax2 <= bx2;
}

bool ObjectLegsCollision(TransForm player, TransForm obj) {
	float ax1 = player.x + 6;
	float ax2 = player.x + player.sizeX - 6;

	//左脚
	float bx1 = obj.x;
	float bx2 = obj.x + 96;

	//右脚
	float cx1 = obj.x + obj.sizeX - 96;
	float cx2 = obj.x + obj.sizeX;

	return (ax1 >= bx1 && ax2 <= bx2) ||
		(ax1 >= cx1 && ax2 <= cx2);
}

Player::Player() {
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 150;
	player.y = WOR_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 7;
	fallFlag = false;
	hideFlag = false;
	menu = new HUD;
	item = new Item;
	object = new Obj;
	enemy = new Enemy;

	scrollX = 0;
	scrollY = 600;

	score = 0;

	spawnType = 0;
	spawnFlag = true;

	LoadDivGraph("resource/pict/player/playermoveright.png", 4, 4, 1, 64, 128, playerRightGH);
	LoadDivGraph("resource/pict/player/playermoveleft.png", 4, 4, 1, 64, 128, playerLeftGH);
	LoadDivGraph("resource/pict/player/playerfallright.png", 4, 4, 1, 64, 192, playerFallRightGH);
	LoadDivGraph("resource/pict/player/playerfallleft.png", 4, 4, 1, 64, 192, playerFallLeftGH);
	LoadDivGraph("resource/pict/player/playerclimb.png", 4, 4, 1, 64, 128, playerClimbGH);
	LoadDivGraph("resource/pict/player/playerhouki.png", 4, 4, 1, 64, 128, playerHoukiGH);
	LoadDivGraph("resource/pict/player/playermop.png", 4, 4, 1, 64, 128, playerMopGH);
	LoadDivGraph("resource/pict/player/playerwait.png", 4, 4, 1, 64, 128, playerWaitGH);
	//playerGH = LoadGraph("resource/pict/player.png");

	walkSE = LoadSoundMem("resource/sound/playerwalk.mp3");	//プレイヤーの歩く音
	ChangeVolumeSoundMem(100, walkSE);
	houkiSE = LoadSoundMem("resource/sound/houki.mp3");
	ChangeVolumeSoundMem(100, houkiSE);
	mopSE = LoadSoundMem("resource/sound/mop.mp3");
	ChangeVolumeSoundMem(100, mopSE);
	trashPointSE = LoadSoundMem("resource/sound/trashpoint.mp3");
	ChangeVolumeSoundMem(100, trashPointSE);
	cleanPointSE = LoadSoundMem("resource/sound/cleanpoint.mp3");
	ChangeVolumeSoundMem(100, cleanPointSE);
	climbSE = LoadSoundMem("resource/sound/climb.mp3");
	ChangeVolumeSoundMem(100, climbSE);
}

Player::~Player() {
	if (menu != NULL) delete menu;
	if (item != NULL) delete item;
	if (object != NULL) delete object;
	if (enemy != NULL) delete enemy;
}

void Player::Reset(int scene) {
	menu->Reset();
	item->Reset();
	player.sizeX = 64;
	player.sizeY = 128;
	player.x = 150;
	player.y = WOR_HEIGHT - player.sizeY;
	spdX = 5;
	spdY = 7;
	fallFlag = false;

	scrollX = 0;
	scrollY = 600;

	score = 0;
	spawnType = 0;
	spawnFlag = true;


	enemy->roomba.x = WIN_WIDTH * 1.5;
	enemy->hitFlag = false;

	//ゲーム画面なら初期スポーン処理
	if (scene == GAME) item->Spawn();
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
	//きれいにするために必要な時間
	const int houkiTime = 50;
	static int houkiTimer = 0;
	const int mopTime = 100;
	static int mopTimer = 0;

	for (int i = 0; i < item->gomi1Num; i++) {
		for (int j = 0; j < menu->inventNum; j++) {
			//ゴミの上で特定のアイテムを持っていればきれいにできる
			if (PlayerItemCollision(player, item->gomi1X[i], item->gomi1Y[i], item->gomi1SizeX, item->gomi1SizeY) &&
				menu->onHandFlag[j] && keys[KEY_INPUT_SPACE] && oldkeys[KEY_INPUT_SPACE]) {
				//食べかすの上でほうき
				if (item->gomi1Type[i] == TABEKASU && menu->itemType[j] == HOUKI) {
					houkiTimer++;
					item->blendNum -= 2;
					item->cleanFlag[i] = true;
					//一定時間spaceを押し続けたらゴミが消えスコアゲット
					if (houkiTimer >= houkiTime) {
						score += 10;
						item->gomi1Type[i] = NONE;
						item->gomi1X[i] = -500;
						houkiTimer = 0;
						PlaySoundMem(cleanPointSE, DX_PLAYTYPE_BACK, true);
						break;
					}
				}
				//油の上でモップ
				else if (item->gomi1Type[i] == ABURA && menu->itemType[j] == MOP) {
					mopTimer++;
					item->blendNum--;
					item->cleanFlag[i] = true;
					//一定時間spaceを押し続けたらゴミが消えスコアゲット
					if (mopTimer >= mopTime) {
						score += 10;
						item->gomi1Type[i] = NONE;
						item->gomi1X[i] = -500;
						mopTimer = 0;
						PlaySoundMem(cleanPointSE, DX_PLAYTYPE_BACK, true);
						break;
					}
				}
			}
			//一度離したらリセット
			else if (!keys[KEY_INPUT_SPACE] && oldkeys[KEY_INPUT_SPACE]) {
				houkiTimer = 0;
				mopTimer = 0;
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
			score += 3;
			menu->itemType[i] = NONE;
			PlaySoundMem(trashPointSE, DX_PLAYTYPE_BACK, true);
			break;
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

//チュートリアル
void Player::Tutorial(char* keys, char* oldkeys, int& tutorialScene) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	//落下処理
	PlayerFall();

	//オブジェクトの脚に触れていたら登れる
	if (ObjLegsDecision(player, object->table) || ObjLegsDecision(player, object->chair)) {
		player.y -= ((float)(keys[KEY_INPUT_W]) - (float)(keys[KEY_INPUT_S])) * spdY;
		fallFlag = false;
	}

	//画面外に出ない
	if (player.x <= 0) player.x = 0;
	else if (player.x >= WIN_WIDTH - player.sizeX) player.x = WIN_WIDTH - player.sizeX;


	if (player.y >= WOR_HEIGHT - player.sizeY) player.y = WOR_HEIGHT - player.sizeY;

	//オブジェクトの上で止まる
	//机
	if (player.x + player.sizeX >= object->table.x && player.x <= object->table.x + object->table.sizeX) {
		if (player.y <= object->table.y - player.sizeY) player.y = object->table.y - player.sizeY + 1;
	}

	////椅子
	//else if (player.x + player.sizeX >= object->chair.x && player.x <= object->chair.x + object->chair.sizeX) {
	//	if (!fallFlag && player.y <= object->chair.y - player.sizeY) player.y = object->chair.y - player.sizeY + 1;
	//}


	//スクロール
	//scrollX = ScrollX(player.x, scrollStartX);
	scrollY = ScrollY(player.y, scrollStartY);

	//プレイヤーの進行度に合わあせてゴミが出現
	if (spawnFlag) {
		item->Tutorial(spawnType);
		spawnFlag = false;
	}

	//ゴミを拾ってゴミ箱に捨てたら次の説明
	if (spawnType == 0 && (score == 9 || tutorialScene == 4)) {
		spawnType = 1;
		spawnFlag = true;
		tutorialScene++;
	}

	//アイテムの取得
	ItemGet(keys, oldkeys);

	//アイテム落とす
	ItemDrop(keys, oldkeys);

	Cleaning(keys, oldkeys);

	//アイテムをゴミ箱に捨てる
	ItemTrash(keys, oldkeys);


	//描画

	//テーブル
	object->TableDraw(scrollX, scrollY);

	//ゴミ箱
	object->TrashBoxDraw(scrollX, scrollY);

	//プレイヤー
	PlayerAniGH(keys, oldkeys);

	//ゴミ
	item->GomiDraw(scrollX, scrollY);

	//HUD
	menu->Craft(keys, oldkeys);
	menu->Inventory(keys, oldkeys);
}


void Player::Option(char* keys, char* oldkeys, int& scene) {
	player.x += ((float)(keys[KEY_INPUT_D]) - (float)(keys[KEY_INPUT_A])) * spdX;

	//落下処理
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

	// 隠れていたらhideFlagをtrueにする
		for (int i = 0; i < 3; i++)
		{
			if (player.y + player.sizeY == WOR_HEIGHT && ObjectCollision(player, object->box[i]))
			{
				hideFlag = true;
				break;
			}
			else if (ObjectLegsCollision(player, object->chair))
			{
				hideFlag = true;
				break;
			}
			else if (ObjectLegsCollision(player, object->table))
			{
				hideFlag = true;
				break;
			}
			else if (ObjectCollision(player, object->kitchen))
			{
				hideFlag = true;
				break;
			}
			else hideFlag = false;
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

	//きれいにする
	Cleaning(keys, oldkeys);

	//アイテムをゴミ箱に捨てる
	ItemTrash(keys, oldkeys);

	//敵の処理
	RoombaCollision(enemy->roomba);
	enemy->EnemyTiming(scrollX, scrollY);
	enemy->HumanSearchCollision(player.x, player.y, player.sizeX, player.sizeY, scrollX, scrollY, hideFlag);
	if (enemy->hitFlag == true)scene = SCORE;
}

void Player::Draw(char* keys, char* oldkeys) {
	//敵（人間）
	if (enemy->humanFlag == true)enemy->HumanDraw(scrollX, scrollY);


	//キッチン
	object->KitchenDraw(scrollX, scrollY);

	//敵（ルンバ）
	if (enemy->roombaFlag == true)enemy->RoombaDraw(scrollX, scrollY);


	//テーブル
	object->TableDraw(scrollX, scrollY);

	//椅子
	object->ChairDraw(scrollX, scrollY);

	//ゴミ箱
	object->TrashBoxDraw(scrollX, scrollY);

	//隠れるためのオブジェクト
	object->BoxDraw(scrollX, scrollY);

	enemy->Warning();

	//プレイヤー
	PlayerAniGH(keys, oldkeys);

	//ゴミ
	item->GomiDraw(scrollX, scrollY);

	//HUD
	menu->Craft(keys, oldkeys);
	menu->Inventory(keys, oldkeys);

	//DrawFormatString(500, 0, 0xffffff, "%d", enemy->hitFlag);
	/*DrawFormatString(530, 0, 0xffffff, "%f", player.y);*/
}

//アニメーション処理
void Player::PlayerAniGH(char* keys, char* oldkeys) {
	int j = playerMove.GHTimer / (playerMove.GHTime / playerMove.GHNum);
	static int GH;
	static int oldGH;
	int add = 0;

	if ((keys[KEY_INPUT_W] || keys[KEY_INPUT_S]) && !fallFlag && (ObjLegsDecision(player, object->table) || ObjLegsDecision(player, object->chair))) {	//上り下り
		playerMove.GHTimer++;
		GH = playerClimbGH[j];
	}
	else if (keys[KEY_INPUT_D]) {	//右に移動中
		playerMove.GHTimer++;
		GH = playerRightGH[j];
		if (fallFlag) {	//落下中
			GH = playerFallRightGH[j];
			oldGH = playerFallRightGH[j];
			add = 64;
		}
	}
	else if (keys[KEY_INPUT_A]) {	//左に移動中
		playerMove.GHTimer++;
		GH = playerLeftGH[j];
		if (fallFlag) {	//落下中
			GH = playerFallLeftGH[j];
			oldGH = playerFallLeftGH[j];
			add = 64;
		}
	}
	else if (keys[KEY_INPUT_SPACE]) {	//掃除
		playerMove.GHTimer++;
		for (int i = 0; i < menu->inventNum; i++) {	//ほうきアニメーション
			if (menu->onHandFlag[i] && menu->itemType[i] == HOUKI) {
				GH = playerHoukiGH[j];
				break;
			}
			else if (menu->onHandFlag[i] && menu->itemType[i] == MOP) {	//モップアニメーション
				GH = playerMopGH[j];
				break;
			}
			else {	//何も持っていないときは待機アニメーション
				GH = playerWaitGH[j];
			}
		}
	}
	else {	//静止中
		playerMove.GHTimer++;
		GH = playerWaitGH[j];
		if (fallFlag) {	//落下中
			GH = oldGH;
			add = 64;
		}
		else { //上り下り中かつ地面に足がついていないとき
			if ((ObjLegsDecision(player, object->table) || ObjLegsDecision(player, object->chair)) && player.y != WOR_HEIGHT - player.sizeY &&
				player.y != object->table.y - player.sizeY + 1 && player.y != object->chair.y - player.sizeY + 1) {
				GH = playerClimbGH[j];
			}
		}
	}

	//アニメーションタイマーリセット
	if (playerMove.GHTimer >= playerMove.GHTime) playerMove.GHTimer = 0;

	DrawGraph(player.x - scrollX, player.y - scrollY - add, GH, true);
}

//プレイヤーとルンバの減速処理(当たり判定)
void Player::RoombaCollision(Object roomba)
{
	if (enemy->roombaFlag)
	{
		if (player.x<roomba.x + roomba.width && player.x + player.sizeX>roomba.x &&
			player.y<roomba.y + roomba.height && player.y + player.sizeY>roomba.y)//X,Yの判定
		{
			spdX = 3;
		}
		else
		{
			spdX = 5;
		}
	}

}

void Player::Sound(char* keys, char* oldkeys) {
	//歩いている音
	if ((keys[KEY_INPUT_D] || keys[KEY_INPUT_A]) && !fallFlag && (!keys[KEY_INPUT_W] && !keys[KEY_INPUT_S])) PlaySoundMem(walkSE, DX_PLAYTYPE_LOOP, false);
	else StopSoundMem(walkSE);

	//掃除する音
	if (keys[KEY_INPUT_SPACE]) {
		for (int i = 0; i < menu->inventNum; i++) {
			if (menu->onHandFlag[i] && menu->itemType[i] == HOUKI) {	//ほうき
				PlaySoundMem(houkiSE, DX_PLAYTYPE_LOOP, false);
			}
			else if (menu->onHandFlag[i] && menu->itemType[i] == MOP) {	//モップ
				PlaySoundMem(mopSE, DX_PLAYTYPE_LOOP, false);
				break;
			}
		}
	}
	else {
		StopSoundMem(houkiSE);
		StopSoundMem(mopSE);
	}

	//登る音
	if ((keys[KEY_INPUT_W] || keys[KEY_INPUT_S]) && (ObjLegsDecision(player, object->table) || ObjLegsDecision(player, object->chair))) {
		PlaySoundMem(climbSE, DX_PLAYTYPE_LOOP, false);
	}
	else {
		StopSoundMem(climbSE);
	}
}

void Player::SoundStop() {
	StopSoundMem(walkSE);
	StopSoundMem(houkiSE);
	StopSoundMem(mopSE);
	StopSoundMem(climbSE);
}