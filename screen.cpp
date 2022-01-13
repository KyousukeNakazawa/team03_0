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


Screen::Screen() {
	backX = 0;
	backY = 0;
	
	for (int i = 0; i < scoreDigits; i++) {
		scoreStrNum[i] = {};
		scoreEachNum[i] = {};
	}

	for (int i = 0; i < timerDigits; i++) {
		timerStrNum[i] = {};
		timerEachNum[i] = {};
	}

	backGH = LoadGraph("resource/pict/background.png");
	LoadDivGraph("resource/pict/scorenum.png", 10, 10, 1, 48, 48, scoreGH);
	LoadDivGraph("resource/pict/timenum.png", 10, 10, 1, 48, 48, timerGH);
}

Screen::~Screen() {

}
void Screen::Background(float scrollX, float scrollY) {
	DrawGraph(backX - scrollX, backY - scrollY, backGH, true);
}

void Screen::ScoreDraw(int score) {
	//�z��Ɋi�[
	sprintf_s(scoreStrNum, sizeof(scoreStrNum), "%05d", score);

	//�I�t�Z�b�g�l�ɍ��킹��
	for (int i = 0; i < scoreDigits; i++) {
		scoreEachNum[i] = scoreStrNum[i] - 48;
	}

	//�`��
	for(int i = 0; i < scoreDigits; i++) DrawGraph(1024 + i * 48, 20, scoreGH[scoreEachNum[i]], true);
	//DrawFormatString(1150, 0, 0xffffff, "%d", score);
}

void Screen::TimerDraw(int timer) {
	//�z��Ɋi�[
	sprintf_s(timerStrNum, sizeof(timerStrNum), "%03d", timer / 60);

	//�I�t�Z�b�g�l�ɍ��킹��
	for (int i = 0; i < timerDigits; i++) {
		timerEachNum[i] = timerStrNum[i] - 48;
	}
	for (int i = 0; i < timerDigits; i++) DrawGraph(608 + i * 48, 20, timerGH[timerEachNum[i]], true);
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

	for (int i = 0; i < maxInvent; i++) {
		onHandFlag[i] = false;
		itemType[i] = NONE;
	}

	inventUpdate = false;

	LoadDivGraph("resource/pict/inventory.png", 2, 2, 1, 96, 96, inventGH);
	LoadDivGraph("resource/pict/gomi/gomi1.png", 8, 4, 4, 64, 64, gomi1GH);
	LoadDivGraph("resource/pict/gomi/gomi2.png", 3, 3, 1, 32, 96, gomi2GH);
	tool1GH = LoadGraph("resource/pict/tool/back.png");
	LoadDivGraph("resource/pict/tool/tool1.png", 2, 2, 1, 32, 96, tool2GH);
	LoadDivGraph("resource/pict/tool/toolcraft1.png", 2, 2, 1, 384, 64, houkicraftGH);
	LoadDivGraph("resource/pict/tool/toolcraft2.png", 2, 2, 1, 384, 64, mopcraftGH);
	LoadDivGraph("resource/pict/tool/toolcraft3.png", 2, 2, 1, 384, 64, backcraftGH);

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

	for (int i = 0; i < maxInvent; i++) {
		onHandFlag[i] = false;
		itemType[i] = NONE;
	}

	inventUpdate = false;
}

void HUD::Craft(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY) {
	int GH[3] = {};
	bool craftFlag[3] = {};
	int houkiCraftCheck = 0;
	int mopCraftCheck = 0;
	int backCraftCheck = 0;

	//�N���t�g���j���[�̕\��
	if (!oldkeys[KEY_INPUT_R] && keys[KEY_INPUT_R]) {
		if (!craftmenuFlag) {
			craftmenuFlag = true;
		}
		else {
			craftmenuFlag = false;
		}
	}


	if (craftmenuFlag) {

		//���V�s��ɃJ�[�\����������摜�ω�
		for (int i = 0; i < craftCategory; i++) {
			if (mouseX > craftX[i] && mouseX < craftX[i] + craftSizeX && mouseY > craftY[i] && mouseY < craftY[i] + craftSizeY) {
				craftFlag[i] = true;
				GH[i] = 1;
			}
		}

		//�C���x���g�����ɑf�ނ������Ă邩�m�F ���łɓ���������Ă�ꍇ�͍��Ȃ�
		for (int i = 0; i < inventNum; i++) {
			//�ق���
			if (itemType[i] == HOUKI) {
				houkiCraftCheck = 0;
				break;
			}
			if (itemType[i] == HOKORI) houkiCraftCheck++;
			if (itemType[i] == ENPITU || itemType[i] == TUMAYOUZI) houkiCraftCheck++;
		}
		for (int i = 0; i < inventNum; i++) {
			//���b�v
			if (itemType[i] == MOP) {
				mopCraftCheck = 0;
				break;
			}
			if (itemType[i] == NUNO) mopCraftCheck++;
			if (itemType[i] == HOUKI) mopCraftCheck++;
		}
		for (int i = 0; i < inventNum; i++) {
			//�o�b�N
			if (inventNum == 6) {
				backCraftCheck = 0;
				break;
			}
			if (itemType[i] == NUNO) backCraftCheck++;
			if (itemType[i] == WAGOMU) backCraftCheck++;
		}
		//�N���b�N�������ɑf�ނ�����΍쐬
		if ((mouse & MOUSE_INPUT_LEFT) && !(oldMouse & MOUSE_INPUT_LEFT)) {
			//�ق���
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
			//���b�v
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
			//�o�b�N
			//���b�v
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
		DrawBox(craftmenu.x, craftmenu.y, craftmenu.x + craftmenu.sizeX, craftmenu.y + craftmenu.sizeY, GetColor(230, 230, 230), true);
		DrawGraph(craftX[0], craftY[0], houkicraftGH[GH[0]], true);
		DrawGraph(craftX[1], craftY[1], mopcraftGH[GH[1]], true);
		DrawGraph(craftX[2], craftY[2], backcraftGH[GH[2]], true);
	}
}

void HUD::Inventory(int& mouse, int& mouseX, int& mouseY) {
	int invGH[6] = {};
	int item1GH[6] = {};	//64*64�̊G
	int item2GH[6] = {};	//32*64�̊G

	//�o�b�N���쐬���ꂽ��z�b�g�o�[��6�ɑ���
	if (inventUpdate) inventNum = maxInvent;
	else inventNum = minInvent;

	//�z�b�g�o�[�̕\�� �N���b�N�Ŏ��A�C�e����I��
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

		//�z�b�g�o�[�Ɏ����Ă�A�C�e����`��
		if (itemType[i] == NONE) {
			item1GH[i] = 0;
			item2GH[i] = 0;
		}
		//���ʂ̃S�~
		for (int j = 0; j < 8; j++) {
			if (itemType[i] == j + 1) item1GH[i] = gomi1GH[j];
		}
		//�ג����S�~
		for (int j = 0; j < 3; j++) {
			if (itemType[i] == j + 9) item2GH[i] = gomi2GH[j];
		}

		//�ג�������
		for (int j = 0; j < 2; j++) {
			if (itemType[i] == j + 12) item2GH[i] = tool2GH[j];
		}

		//�g
		DrawGraph(invent.x + invent.sizeX * i, invent.y, invGH[i], true);

		//�A�C�e���A�C�R��
		//���ʂ̃S�~
		DrawExtendGraph(invent.x + 4 + invent.sizeX * i, invent.y + 4, invent.x + (invent.sizeX - 4) * (i + 1), invent.y + invent.sizeY - 4, item1GH[i], true);
		//�ג����A�C�e��
		DrawGraph(invent.x + 32 + invent.sizeX * i, invent.y, item2GH[i], true);
	}
}