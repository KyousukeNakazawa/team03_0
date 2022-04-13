#pragma once
#include "screen.h"

class Obj {
private:

	int trashBoxGH;
	int kitchenGH;
	int tableGH;
	int chairGH;
	int boxGH;

public:
	TransForm trashBox;
	TransForm kitchen;
	TransForm table;
	TransForm chair;
	int chairGraphY;
	TransForm box[3];
	const int boxNum = 3;

	Obj();

	void TrashBoxDraw(float scrollX, float scrollY);
	void KitchenDraw(float scrollX, float scrollY);
	void TableDraw(float scrollX, float scrollY);
	void ChairDraw(float scrollX, float scrollY);
	void BoxDraw(float scrollX, float scrollY);
};