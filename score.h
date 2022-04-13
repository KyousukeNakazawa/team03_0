#pragma once
#include "screen.h"

class Score {
private:
	//スコアのセーブ
	FILE* rfp;
	FILE* wfp;

	int error;// ファイルを読み込めたか
	char data[100];// ファイルから読み込んだデータ(文字列)
	int arr[3];// ファイルから読み込んだデータ(整数型)

	const int digits = 6;

	char strNumSave[6] = { 0 };
	int eachNumSave[6] = { 0 };

	char strNumSave1[6] = { 0 };
	int eachNumSave1[6] = { 0 };

	char strNumSave2[6] = { 0 };
	int eachNumSave2[6] = { 0 };

	char strNumSave3[6] = { 0 };
	int eachNumSave3[6] = { 0 };

	int numberGH[10];
	int backGH;
	int backX;
	int backY;

	TransForm scoreNum;


public:
	Score();

	void Ranking(int score);
};