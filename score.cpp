#include "DxLib.h"
#include "score.h"
#include "enum.h"

Score::Score() {
	rfp = NULL;
	wfp = NULL;
	backX = 0;
	backY = 0;
	scoreNum.sizeX = 48;
	scoreNum.sizeY = 115;
	scoreNum.x = 520;
	scoreNum.y = 440;
	LoadDivGraph("resource/pict/scorenum.png", 10, 10, 1, 48, 48, numberGH);
	backGH = LoadGraph("resource/pict/score.png");
}

void Score::Ranking(int score){
	error = fopen_s(&rfp, "resource/save.txt", "r");

	int digitCount = 0;// 何行読み込んだか
	while (fgets(data, 100, rfp) != NULL)
	{
		int tmp = atoi(data);// ascii to int
		arr[digitCount] = tmp;
		digitCount++;
	}

	fclose(rfp);// 閉じる
	// ---------- スコアを更新 ----------
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
	// ---------- ファイルに更新した値を書き込む(write) ----------
	error = fopen_s(&wfp, "resource/save.txt", "w");

	// ファイルに書き込み
	for (int i = 0; i < 3; ++i)
	{
		fprintf(wfp, "%d\n", arr[i]);
	}

	fclose(wfp);// 閉じる

	//スコアランキング描画
	sprintf_s(strNumSave, sizeof(strNumSave1), "%05d", score);
	sprintf_s(strNumSave1, sizeof(strNumSave1), "%05d", arr[0]);
	sprintf_s(strNumSave2, sizeof(strNumSave2), "%05d", arr[1]);
	sprintf_s(strNumSave3, sizeof(strNumSave3), "%05d", arr[2]);

	//オフセット値に合わせる
	for (int i = 0; i < digits; i++) {
		eachNumSave[i] = strNumSave[i] - 48;
		eachNumSave1[i] = strNumSave1[i] - 48;
		eachNumSave2[i] = strNumSave2[i] - 48;
		eachNumSave3[i] = strNumSave3[i] - 48;
	}

	//描画
	DrawGraph(backX, backY, backGH, true);

	for (int i = 0; i < digits; i++) {
		//ゲームスコア
		DrawGraph(scoreNum.x + i * scoreNum.sizeX, 330, numberGH[eachNumSave[i]], true);

		//スコアランキング
		DrawGraph(scoreNum.x + i * scoreNum.sizeX, scoreNum.y, numberGH[eachNumSave1[i]], true);
		DrawGraph(scoreNum.x + i * scoreNum.sizeX, scoreNum.y + scoreNum.sizeY, numberGH[eachNumSave2[i]], true);
		DrawGraph(scoreNum.x + i * scoreNum.sizeX, scoreNum.y + scoreNum.sizeY * 2, numberGH[eachNumSave3[i]], true);
	}
	
}