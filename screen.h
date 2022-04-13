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

//�A�j���[�V�����p
struct AniGHTimer {
	const int GHNum;
	const int GHTime;
	int GHTimer;
};

float ScrollX(float& playerPos, const float scrollStart);
float ScrollY(float& playerPos, const float scrollStart);

void AniStop(AniGHTimer& Ani, int time);

class Screen {
private:
	//�w�i
	float backX;
	float backY;
	int backGH;

	//���[�h���
	float loadX;
	float loadY;
	int loading[4];
	int loading2[4];
	AniGHTimer load = { 4, 120, 0 };
	
	//�����摜���p
	const int scoreDigits = 6;
	char scoreStrNum[6];
	int scoreEachNum[6];

	const int timerDigits = 3;
	char timerStrNum[4];
	int timerEachNum[4];

	int scoreGH[10];
	int timerGH[10];

	//��������A�j���[�V����
	
	int optionAni1[9];
	int optionAni2[9];
	int optionAni3[8];
	int optionAni4[12];
	int optionAni5[2];
	int optionAni6[3];
	int optionAni7[2];
	int optionAni8[4];
	AniGHTimer optionAni = { 12, 180, 0 };

	

public:
	int tutorialScene;

	Screen();
	~Screen();

	void Load(int& scene, int stage);
	void Background(float scrollX, float scrollY);
	void ScoreDraw(int score);
	void TimerDraw(int timer);
	void TutorialAniGH(char* keys, char* oldkeys);
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
	int invSelect;
	int craftSelect;


	int inventGH[2];
	int gomi1GH[8];
	int gomi2GH[3];
	int tool1GH;
	int tool2GH[2];
	int houkicraftGH[2];
	int mopcraftGH[2];
	int backcraftGH[3];
	int craftBack;

public:
	int inventNum;

	bool onHandFlag[6];
	int itemType[6];

	HUD();
	~HUD();
	void Craft(char* keys, char* oldkeys);
	void Inventory(char* keys, char* oldkeys);
	void Reset();
};