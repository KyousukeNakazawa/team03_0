#pragma once


enum Scene {
	TITLE,
	TUTORIAL,
	GAME,
	SCORE,
	LOAD,
};


enum HandInfo {
	off_hand,
	on_hand
};

enum ItemInfo {
	NONE,		//0
	
	//�S�~64*64
	ABURA,		//1
	TABEKASU,	//2
	HOKORI,		//3
	CAN,		//4
	CASSETT,	//5
	NAMAGOMI,	//6
	NUNO,		//7
	WAGOMU,		//8

	//�S�~32*96
	ENPITU,		//9
	STRAW,		//10
	TUMAYOUZI,	//11

	//����
	HOUKI,		//12
	MOP,		//13
	BACK,		//14
	BALL,		//15
};