#pragma once
#include "screen.h"

class Obj {
private:

	int trashBoxGH;
	int kitchenGH;
	int tableGH;
	int chairGH;

public:
	TransForm trashBox;
	TransForm kitchen;
	TransForm table;
	TransForm chair;
	int chairGraphY;

	Obj();

	void TrashBoxDraw(float scrollX, float scrollY);
	void KitchenDraw(float scrollX, float scrollY);
	void TableDraw(float scrollX, float scrollY);
	void ChairDraw(float scrollX, float scrollY);
};