#include "DxLib.h"
#include "object.h"
#include"screen.h"
#include"enum.h"

Obj::Obj() {
	trashBox.sizeX = 96;
	trashBox.sizeY = 96;
	trashBox.x = 30;
	trashBox.y = WOR_HEIGHT - trashBox.sizeY;

	kitchen.sizeX = 350;
	kitchen.sizeY = 1000;
	kitchen.x = 0;
	kitchen.y = WOR_HEIGHT - kitchen.sizeY;

	table.sizeX = 800;
	table.sizeY = 1000;
	table.x = 1000;
	table.y = WOR_HEIGHT - table.sizeY;

	chair.sizeX = 300;
	chair.sizeY = 600;
	chair.x = 1900;
	chair.y = WOR_HEIGHT - chair.sizeY;
	chairGraphY = 300;

	trashBoxGH = LoadGraph("resource/pict/object/gomibako.png");
	kitchenGH = LoadGraph("resource/pict/object/kitchen.png");
	tableGH = LoadGraph("resource/pict/object/table.png");
	chairGH = LoadGraph("resource/pict/object/chair.png");
}

void Obj::TrashBoxDraw(float scrollX, float scrollY) {
	DrawGraph(trashBox.x - scrollX, trashBox.y - scrollY, trashBoxGH, true);
}

void Obj::KitchenDraw(float scrollX, float scrollY) {
	DrawGraph(kitchen.x - scrollX, kitchen.y - scrollY, kitchenGH, true);
}

void Obj::TableDraw(float scrollX, float scrollY) {
	DrawGraph(table.x - scrollX, table.y - scrollY, tableGH, true);
}

void Obj::ChairDraw(float scrollX, float scrollY) {
	DrawGraph(chair.x - scrollX, chairGraphY - scrollY, chairGH, true);
}