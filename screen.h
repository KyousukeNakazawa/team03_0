#pragma once

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 800;

//�I�u�W�F�N�g��{���
struct TransForm {
	float x;
	float y;
	float sizeX;
	float sizeY;
};

class Menu {
private:
	TransForm craft;
	bool menuFlag;

public:
	Menu();
	~Menu();
	void Craft(char* keys, char* oldkeys);
};