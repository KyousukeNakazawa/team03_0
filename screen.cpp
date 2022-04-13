#include "DxLib.h"
#include "screen.h"
#include "enum.h"

float ScrollX(float& playerPos, const float scrollStart) {
	if (playerPos > WOR_WIDTH - scrollStart) {
		return WOR_WIDTH - scrollStart * 2;
	}
	else if (playerPos < scrollStart) {
		return 0;
	}
	else {
		return playerPos - scrollStart;
	}
}

float ScrollY(float& playerPos, const float scrollStart) {
	if (playerPos < scrollStart) {
		return 0;
	}
	else if (playerPos > WOR_HEIGHT - scrollStart) {
		return 600;
	}
	else {
		return playerPos - scrollStart;
	}
}

void AniStop(AniGHTimer& Ani, int time) {
	//アニメーションが一枚進むフレーム数を計算
	int frame = Ani.GHTime / Ani.GHNum;
	static int count = 0;
	if (Ani.GHTimer >= time - 1) {
		count++;
		Ani.GHTimer = time - 1;
		if (count >= frame) {
			Ani.GHTimer = 0;
			count = 0;
		}
	}
}

Screen::Screen() {
	backX = 0;
	backY = 0;
	loadX = 0;
	loadY = 0;

	for (int i = 0; i < scoreDigits; i++) {
		scoreStrNum[i] = {};
		scoreEachNum[i] = {};
	}

	for (int i = 0; i < timerDigits; i++) {
		timerStrNum[i] = {};
		timerEachNum[i] = {};
	}

	tutorialScene = 0;

	backGH = LoadGraph("resource/pict/background.png");
	LoadDivGraph("resource/pict/scorenum.png", 10, 10, 1, 48, 48, scoreGH);
	LoadDivGraph("resource/pict/timenum.png", 10, 10, 1, 48, 48, timerGH);

	LoadDivGraph("resource/pict/sousa1.png", 9, 9, 1, 576, 400, optionAni1);
	LoadDivGraph("resource/pict/sousa2.png", 9, 9, 1, 576, 400, optionAni2);
	LoadDivGraph("resource/pict/sousa3.png", 8, 8, 1, 576, 400, optionAni3);
	LoadDivGraph("resource/pict/sousa4.png", 12, 12, 1, 576, 400, optionAni4);
	LoadDivGraph("resource/pict/sousa5.png", 2, 2, 1, 576, 400, optionAni5);
	LoadDivGraph("resource/pict/sousa6.png", 3, 3, 1, 576, 400, optionAni6);
	LoadDivGraph("resource/pict/sousa7.png", 2, 2, 1, 576, 400, optionAni7);
	LoadDivGraph("resource/pict/sousa8.png", 4, 4, 1, 576, 400, optionAni8);
	LoadDivGraph("resource/pict/loading.png", 4, 4, 1, WIN_WIDTH, WIN_HEIGHT, loading);
	LoadDivGraph("resource/pict/loading2.png", 4, 4, 1, WIN_WIDTH, WIN_HEIGHT, loading2);
}

Screen::~Screen() {

}

void Screen::Load(int& scene, int stage) {
	static int loadCut = 0;
	int j = (load.GHTimer % 60) / (load.GHTime / 2 / load.GHNum);

	//描画
	if (loadCut % 2 == 0) DrawGraph(0, 0, loading[j], true);
	else DrawGraph(0, 0, loading2[j], true);

	
	load.GHTimer++;
	if (load.GHTimer >= load.GHTime) {
		load.GHTimer = 0;
		scene = stage;
		loadCut++;
	}

	
}

void Screen::Background(float scrollX, float scrollY) {
	DrawGraph(backX - scrollX, backY - scrollY, backGH, true);
}

void Screen::ScoreDraw(int score) {
	//配列に格納
	sprintf_s(scoreStrNum, sizeof(scoreStrNum), "%05d", score);

	//オフセット値に合わせる
	for (int i = 0; i < scoreDigits; i++) {
		scoreEachNum[i] = scoreStrNum[i] - 48;
	}

	//描画
	for (int i = 0; i < scoreDigits; i++) DrawGraph(1024 + i * 48, 20, scoreGH[scoreEachNum[i]], true);
	//DrawFormatString(1150, 0, 0xffffff, "%d", score);
}

void Screen::TimerDraw(int timer) {
	//配列に格納
	sprintf_s(timerStrNum, sizeof(timerStrNum), "%03d", timer / 60);

	//オフセット値に合わせる
	for (int i = 0; i < timerDigits; i++) {
		timerEachNum[i] = timerStrNum[i] - 48;
	}
	for (int i = 0; i < timerDigits; i++) DrawGraph(608 + i * 48, 20, timerGH[timerEachNum[i]], true);
}

void Screen::TutorialAniGH(char* keys, char* oldkeys) {
	int j = optionAni.GHTimer / (optionAni.GHTime / optionAni.GHNum);
	const int pages9GHTime = 135;
	const int pages8GHTime = 120;
	const int pages4GHTime = 60;
	const int pages3GHTime = 45;
	const int pages2GHTime = 30;
	static int aniGH = optionAni1[j];

	//Gキーを押したら次の説明
	if (keys[KEY_INPUT_G] && !oldkeys[KEY_INPUT_G]) {
		tutorialScene++;
		optionAni.GHTimer = 0;
		if (tutorialScene > 7) tutorialScene = 7;
	}
	else if (keys[KEY_INPUT_B] && !oldkeys[KEY_INPUT_B]) {
		tutorialScene--;
		optionAni.GHTimer = 0;
		if (tutorialScene < 0) tutorialScene = 0;
	}

	//操作説明画像
	optionAni.GHTimer++;
	//プレイヤーの進行度に合わせて画像を表示
	if (tutorialScene == 0 || tutorialScene == 1) {
		if (tutorialScene == 0) aniGH = optionAni1[j];
		else aniGH = optionAni2[j];
		if (optionAni.GHTimer >= pages9GHTime) optionAni.GHTimer = 0;	//アニメーションタイマーリセット
	}
	else if (tutorialScene == 2) {
		aniGH = optionAni3[j];
		AniStop(optionAni, pages8GHTime);
	}
	else if (tutorialScene == 3) {
		aniGH = optionAni4[j];
		AniStop(optionAni, optionAni.GHTime);
	}
	else if (tutorialScene == 4 || tutorialScene == 6) {
		if (tutorialScene == 4) aniGH = optionAni5[1];
		else aniGH = optionAni7[j];
		AniStop(optionAni, pages2GHTime);
	}
	else if (tutorialScene == 5) {
		aniGH = optionAni6[j];
		if (optionAni.GHTimer >= pages3GHTime) optionAni.GHTimer = 0;
	}
	else {
		aniGH = optionAni8[j];
		if (optionAni.GHTimer >= pages4GHTime) optionAni.GHTimer = 0;
	}

	DrawGraph(413, 120, aniGH, true);
}


HUD::HUD() {
	craftmenu.sizeX = 384;
	craftmenu.sizeY = 250;
	craftmenu.x = 20;
	craftmenu.y = 150;
	craftmenuFlag = false;

	craftSizeX = 384;
	craftSizeY = 64;
	for (int i = 0; i < craftCategory; i++) {
		craftX[i] = craftmenu.x;
		craftY[i] = craftmenu.y + 15 + ((craftSizeY + 10) * i);
	}

	invent.x = 20;
	invent.y = 20;
	invent.sizeX = 96;
	invent.sizeY = 96;
	inventNum = minInvent;
	invSelect = 0;
	craftSelect = 0;

	for (int i = 0; i < maxInvent; i++) {
		onHandFlag[i] = false;
		itemType[i] = NONE;
	}
	onHandFlag[0] = true;

	inventUpdate = false;

	LoadDivGraph("resource/pict/inventory.png", 2, 2, 1, 96, 96, inventGH);
	LoadDivGraph("resource/pict/gomi/gomi1.png", 8, 4, 4, 64, 64, gomi1GH);
	LoadDivGraph("resource/pict/gomi/gomi2.png", 3, 3, 1, 32, 96, gomi2GH);
	tool1GH = LoadGraph("resource/pict/tool/back.png");
	LoadDivGraph("resource/pict/tool/tool1.png", 2, 2, 1, 32, 96, tool2GH);
	LoadDivGraph("resource/pict/tool/toolcraft1.png", 2, 2, 1, 384, 64, houkicraftGH);
	LoadDivGraph("resource/pict/tool/toolcraft2.png", 2, 2, 1, 384, 64, mopcraftGH);
	LoadDivGraph("resource/pict/tool/toolcraft4.png", 3, 3, 1, 384, 64, backcraftGH);
	craftBack = LoadGraph("resource/pict/craftmenu.png");

}

HUD::~HUD() {

}

void HUD::Reset() {
	craftmenu.sizeX = 384;
	craftmenu.sizeY = 250;
	craftmenu.x = 20;
	craftmenu.y = 150;
	craftmenuFlag = false;

	craftSizeX = 384;
	craftSizeY = 64;
	for (int i = 0; i < craftCategory; i++) {
		craftX[i] = craftmenu.x;
		craftY[i] = craftmenu.y + 15 + ((craftSizeY + 10) * i);
	}

	invent.x = 20;
	invent.y = 20;
	invent.sizeX = 96;
	invent.sizeY = 96;
	inventNum = minInvent;
	invSelect = 0;
	craftSelect = 0;

	for (int i = 0; i < maxInvent; i++) {
		onHandFlag[i] = false;
		itemType[i] = NONE;
	}

	inventUpdate = false;
}

void HUD::Craft(char* keys, char* oldkeys) {
	int GH[3] = { 1 };
	bool craftFlag[3] = { 1 };
	int houkiCraftCheck = 0;
	int mopCraftCheck = 0;
	int backCraftCheck = 0;

	//クラフトメニューの表示
	if (!oldkeys[KEY_INPUT_R] && keys[KEY_INPUT_R]) {
		if (!craftmenuFlag) {
			craftmenuFlag = true;
		}
		else {
			craftmenuFlag = false;
		}
	}


	if (craftmenuFlag) {
		//十字キー処理
		if (keys[KEY_INPUT_DOWN] && !oldkeys[KEY_INPUT_DOWN]) {	//下
			craftSelect++;
			if (!inventUpdate) {
				if (craftSelect >= craftCategory) craftSelect = 0;
			}
			else {	//バックを作ったあとは選択できないように
				if (craftSelect >= craftCategory - 1) craftSelect = 0;
			}
		}
		else if (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP]) {	//上
			craftSelect--;
			if (craftSelect < 0) {
				if (!inventUpdate) craftSelect = craftCategory - 1;
				else craftSelect = craftCategory - 2;
			}
		}

		//クラフト選択移動処理
		if (!craftFlag[craftSelect]) {
			for (int j = 0; j < craftCategory; j++) {
				craftFlag[j] = false;
				GH[j] = 0;
			}
			craftFlag[craftSelect] = true;
			GH[craftSelect] = 1;
		}


		//レシピ上にカーソルが来たら画像変化
		/*for (int i = 0; i < craftCategory; i++) {
			if (mouseX > craftX[i] && mouseX < craftX[i] + craftSizeX && mouseY > craftY[i] && mouseY < craftY[i] + craftSizeY) {
				craftFlag[i] = true;
				GH[i] = 1;
			}
		}*/

		//インベントリ欄に素材が揃ってるか確認 すでに道具を持ってる場合は作れない
		for (int i = 0; i < inventNum; i++) {
			//ほうき
			if (itemType[i] == HOUKI) {
				houkiCraftCheck = 0;
				break;
			}
			if (itemType[i] == HOKORI) houkiCraftCheck++;
			if (itemType[i] == ENPITU || itemType[i] == TUMAYOUZI) houkiCraftCheck++;
		}
		for (int i = 0; i < inventNum; i++) {
			//モップ
			if (itemType[i] == MOP) {
				mopCraftCheck = 0;
				break;
			}
			if (itemType[i] == NUNO) mopCraftCheck++;
			if (itemType[i] == HOUKI) mopCraftCheck++;
		}
		for (int i = 0; i < inventNum; i++) {
			//バック
			if (inventNum == 6) {
				backCraftCheck = 0;
				break;
			}
			if (itemType[i] == NUNO) backCraftCheck++;
			if (itemType[i] == WAGOMU) backCraftCheck++;
		}
		//ボタンを押したした時に素材があれば作成
		if (keys[KEY_INPUT_F] && !oldkeys[KEY_INPUT_F]) {
			//ほうき
			if (craftFlag[0] && houkiCraftCheck >= 2) {
				for (int i = 0; i < inventNum; i++) {
					if (itemType[i] == HOKORI) {
						itemType[i] = HOUKI;
						break;
					}
				}
				for (int i = 0; i < inventNum; i++) {
					if (itemType[i] == ENPITU || itemType[i] == TUMAYOUZI) {
						itemType[i] = NONE;
						break;
					}
				}
			}
			//モップ
			if (craftFlag[1] && mopCraftCheck >= 2) {
				for (int i = 0; i < inventNum; i++) {
					if (itemType[i] == NUNO) {
						itemType[i] = MOP;
						break;
					}
				}
				for (int i = 0; i < inventNum; i++) {
					if (itemType[i] == HOUKI) {
						itemType[i] = NONE;
						break;
					}
				}
			}
			//バック
			//モップ
			if (craftFlag[2] && backCraftCheck >= 2) {
				for (int i = 0; i < inventNum; i++) {
					if (itemType[i] == NUNO) {
						itemType[i] = NONE;
						break;
					}
				}
				for (int i = 0; i < inventNum; i++) {
					if (itemType[i] == WAGOMU) {
						itemType[i] = NONE;
						break;
					}
				}
				inventUpdate = true;
			}
		}
	}

	if (craftmenuFlag) {
		//DrawGraph();
		DrawGraph(craftmenu.x, craftmenu.y, craftBack, true);
		//DrawBox(craftmenu.x, craftmenu.y + craftmenu.sizeY, craftmenu.x + craftmenu.sizeX, craftmenu.y + craftmenu.sizeY + 50, 0xffffff, true);
		DrawGraph(craftX[0], craftY[0], houkicraftGH[GH[0]], true);
		DrawGraph(craftX[1], craftY[1], mopcraftGH[GH[1]], true);
		if (!inventUpdate) DrawGraph(craftX[2], craftY[2], backcraftGH[GH[2]], true);
		else DrawGraph(craftX[2], craftY[2], backcraftGH[2], true);
	}
}

void HUD::Inventory(char* keys, char* oldkeys) {
	int invGH[6] = {};
	int item1GH[6] = {};	//64*64の絵
	int item2GH[6] = {};	//32*64の絵

	//バックが作成されたらホットバーが6つに増加
	if (inventUpdate) inventNum = maxInvent;
	else inventNum = minInvent;

	//十字キー処理
	if (keys[KEY_INPUT_RIGHT] && !oldkeys[KEY_INPUT_RIGHT]) {	//右
		invSelect++;
		if (invSelect >= inventNum) invSelect = 0;
	}
	else if (keys[KEY_INPUT_LEFT] && !oldkeys[KEY_INPUT_LEFT]) {	//左
		invSelect--;
		if (invSelect < 0) invSelect = inventNum - 1;
	}

	//アイテム選択移動処理
	if (!onHandFlag[invSelect]) {
		for (int j = 0; j < inventNum; j++) {
			onHandFlag[j] = false;
		}
		onHandFlag[invSelect] = true;
	}

	//ホットバーの表示 クリックで持つアイテムを選択
	for (int i = 0; i < inventNum; i++) {
		invGH[i] = inventGH[off_hand];
		//マウス処理一応残してます
		/*if (mouseY > invent.y && mouseY < invent.y + invent.sizeY &&
			mouseX > invent.x + invent.sizeX * i && mouseX < invent.x + invent.sizeX * (i + 1)) {
			if ((mouse & MOUSE_INPUT_LEFT) && !onHandFlag[i]) {
				for (int j = 0; j < inventNum; j++) {
					onHandFlag[j] = false;
				}
				onHandFlag[i] = true;
			}
		}*/

		if (onHandFlag[i]) {
			invGH[i] = inventGH[on_hand];
		}
		else {
			invGH[i] = inventGH[off_hand];
		}

		//ホットバーに持ってるアイテムを描画
		if (itemType[i] == NONE) {
			item1GH[i] = 0;
			item2GH[i] = 0;
		}
		//普通のゴミ
		for (int j = 0; j < 8; j++) {
			if (itemType[i] == j + 1) item1GH[i] = gomi1GH[j];
		}
		//細長いゴミ
		for (int j = 0; j < 3; j++) {
			if (itemType[i] == j + 9) item2GH[i] = gomi2GH[j];
		}

		//細長い道具
		for (int j = 0; j < 2; j++) {
			if (itemType[i] == j + 12) item2GH[i] = tool2GH[j];
		}

		//枠
		DrawGraph(invent.x + invent.sizeX * i, invent.y, invGH[i], true);

		//アイテムアイコン
		//普通のゴミ
		DrawExtendGraph(invent.x + 4 + invent.sizeX * i, invent.y + 4, invent.x + (invent.sizeX - 4) * (i + 1), invent.y + invent.sizeY - 4, item1GH[i], true);
		//細長いアイテム
		DrawGraph(invent.x + 32 + invent.sizeX * i, invent.y, item2GH[i], true);
	}
}