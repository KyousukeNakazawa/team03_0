#pragma once
#include "screen.h"

class Score {
private:
	//�X�R�A�̃Z�[�u
	FILE* rfp;
	FILE* wfp;

	int error;// �t�@�C����ǂݍ��߂���
	char data[100];// �t�@�C������ǂݍ��񂾃f�[�^(������)
	int arr[3];// �t�@�C������ǂݍ��񂾃f�[�^(�����^)

	const int digits = 6;

	char strNumSave1[6] = { 0 };
	int eachNumSave1[6] = { 0 };

	char strNumSave2[6] = { 0 };
	int eachNumSave2[6] = { 0 };

	char strNumSave3[6] = { 0 };
	int eachNumSave3[6] = { 0 };

	int numberGH[10];

public:
	Score();

	void Ranking(int score);
};