#include "DxLib.h"
#include "screen.h"
#include "enum.h"

int Scroll(float& playerPos, float& speed, const float scrollStart) {
	if (playerPos > WOR_WIDTH - scrollStart) {
		return WIN_WIDTH * 2;
	}
	else if (playerPos < scrollStart) {
		return 0;
	}
	else {
		return playerPos - scrollStart;
	}
}

Screen::Screen() {
	backX = 0;
	backY = 0;

	backGH = LoadGraph("resource/pict/background.png");
}

Screen::~Screen() {

}
void Screen::Background(int scrollX) {
	DrawGraph(backX - scrollX, backY, backGH, true);
}

void Screen::ScoreDraw(int scrollX, int score) {
	//DrawGraph();
	DrawFormatString(1200, 0, 0xffffff, "%d", score);
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
			if (itemType[i] == BACK) {
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
		DrawExtendGraph(invent.x + invent.sizeX * i, invent.y, invent.x + invent.sizeX * (i + 1), invent.y + invent.sizeY, item1GH[i], true);
		//�ג����A�C�e��
		DrawGraph(invent.x + 32 + invent.sizeX * i, invent.y, item2GH[i], true);
	}
}