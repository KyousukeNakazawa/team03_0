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

	for (int i = 0; i < boxNum; i++) {
		box[i].sizeX = 128;
		box[i].sizeY = 128;
	}
	box[0].x = 600;
	box[0].y = WOR_HEIGHT - box[0].sizeY;
	box[1].x = 1350;
	box[1].y = WOR_HEIGHT - box[1].sizeY;
	box[2].x = 2350;
	box[2].y = WOR_HEIGHT - box[2].sizeY;

	trashBoxGH = LoadGraph("resource/pict/object/gomibako.png");
	kitchenGH = LoadGraph("resource/pict/object/kitchen.png");
	tableGH = LoadGraph("resource/pict/object/table.png");
	chairGH = LoadGraph("resource/pict/object/chair.png");
	boxGH = LoadGraph("resource/pict/object/smollbox2.png");
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

void Obj::BoxDraw(float scrollX, float scrollY) {
	for (int i = 0; i < boxNum; i++) {
		DrawGraph(box[i].x - scrollX, box[i].y - scrollY, boxGH, true);
	}
}