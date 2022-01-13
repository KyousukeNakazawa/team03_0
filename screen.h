#pragma once
#include "item.h"

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 800;

const int WOR_WIDTH = 2560;

const int WOR_HEIGHT = 1400;

//�I�u�W�F�N�g��{���
struct TransForm {
	float x;
	float y;
	float sizeX;
	float sizeY;
};

struct Object
{
	float x;
	float y;
	float width;
	float height;
	float speed;
};

float ScrollX(float& playerPos, const float scrollStart);
float ScrollY(float& playerPos, const float scrollStart);

class Screen {
private:
	float backX;
	float backY;
	
	//�����摜���p
	const int scoreDigits = 6;
	char scoreStrNum[6];
	int scoreEachNum[6];

	const int timerDigits = 4;
	char timerStrNum[4];
	int timerEachNum[4];

	int backGH;
	int scoreGH[10];
	int timerGH[10];

public:
	

	Screen();
	~Screen();

	void Background(float scrollX, float scrollY);
	void ScoreDraw(int score);
	void TimerDraw(int timer);
};

class HUD {
private:
	//�N���t�g���j���[
	TransForm craftmenu;
	bool craftmenuFlag;

	//�N���t�g���j���[���̏��
	const int craftCategory = 3;
	int craftX[3];
	int craftY[3];
	int craftSizeX = 384;
	int craftSizeY = 384;

	//�z�b�g�o�[
	const int minInvent = 4;
	const int maxInvent = 6;
	bool inventUpdate;
	TransForm invent;


	int inventGH[2];
	int gomi1GH[8];
	int gomi2GH[3];
	int tool1GH;
	int tool2GH[2];
	int houkicraftGH[2];
	int mopcraftGH[2];
	int backcraftGH[2];

public:
	int inventNum;

	bool onHandFlag[6];
	int itemType[6];

	HUD();
	~HUD();
	void Craft(char* keys, char* oldkeys, int& mouse, int& oldMouse, int& mouseX, int& mouseY);
	void Inventory(int& mouse, int& mouseX, int& mouseY);
	void Reset();
};