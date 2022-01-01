#include "DxLib.h"
#include "object.h"
#include"screen.h"
#include"enum.h"

Obj::Obj() {
	trashBox.sizeX = 96;
	trashBox.sizeY = 96;
	trashBox.x = 30;
	trashBox.y = WIN_HEIGHT - trashBox.sizeY;

	trashBoxGH = LoadGraph("resource/pict/gomibako.png");
}

void Obj::TrashBoxDraw(float scrollX) {
	DrawGraph(trashBox.x - scrollX, trashBox.y, trashBoxGH, true);
}