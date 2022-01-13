#include "DxLib.h"
#include "score.h"
#include "enum.h"

Score::Score() {
	rfp = NULL;
	wfp = NULL;
	LoadDivGraph("resource/pict/scorenum.png", 10, 10, 1, 48, 48, numberGH);
}

void Score::Ranking(int score){
	error = fopen_s(&rfp, "resource/save.txt", "r");

	int digitCount = 0;// ���s�ǂݍ��񂾂�
	while (fgets(data, 100, rfp) != NULL)
	{
		int tmp = atoi(data);// ascii to int
		arr[digitCount] = tmp;
		digitCount++;
	}

	fclose(rfp);// ����
	// ---------- �X�R�A���X�V ----------
	if (arr[0] < score)
	{
		arr[2] = arr[1];
		arr[1] = arr[0];
		arr[0] = score;
	}
	else if (arr[1] < score && arr[0] > score)
	{
		arr[2] = arr[1];
		arr[1] = score;
	}
	else if (arr[2] < score && arr[1] > score)
	{
		arr[2] = score;
	}
	// ---------- �t�@�C���ɍX�V�����l����������(write) ----------
	error = fopen_s(&wfp, "resource/save.txt", "w");

	// �t�@�C���ɏ�������
	for (int i = 0; i < 3; ++i)
	{
		fprintf(wfp, "%d\n", arr[i]);
	}

	fclose(wfp);// ����

	//�X�R�A�����L���O�`��
	sprintf_s(strNumSave1, sizeof(strNumSave1), "%05d", arr[0]);
	sprintf_s(strNumSave2, sizeof(strNumSave2), "%05d", arr[1]);
	sprintf_s(strNumSave3, sizeof(strNumSave3), "%05d", arr[2]);

	//�I�t�Z�b�g�l�ɍ��킹��
	for (int i = 0; i < digits; i++) {
		eachNumSave1[i] = strNumSave1[i] - 48;
		eachNumSave2[i] = strNumSave2[i] - 48;
		eachNumSave3[i] = strNumSave3[i] - 48;
	}

	//�X�R�A
	for (int i = 0; i < digits; i++) {
		DrawGraph(48 * i + 700, 515, numberGH[eachNumSave1[i]], true);
		DrawGraph(48 * i + 700, 570, numberGH[eachNumSave2[i]], true);
		DrawGraph(48 * i + 700, 625, numberGH[eachNumSave3[i]], true);
	}
}