#include "DxLib.h"
#include "screen.h"
#include "player.h"
#include "enum.h"
#include "score.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

// ウィンドウのタイトルに表示する文字列
const char WIN_TITLE[] = "こびとのそうじめいじん";

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(WIN_TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0xff, 0xff, 0xff);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み
	int titleBGM = LoadSoundMem("resource/sound/title.mp3");
	ChangeVolumeSoundMem(100, titleBGM);
	int stageBGM = LoadSoundMem("resource/sound/stage.mp3");
	ChangeVolumeSoundMem(100, stageBGM);
	int tutorialBGM = LoadSoundMem("resource/sound/tutorial.mp3");
	ChangeVolumeSoundMem(100, tutorialBGM);
	int scoreBGM = LoadSoundMem("resource/sound/score.mp3");
	ChangeVolumeSoundMem(100, scoreBGM);
	int selectSE = LoadSoundMem("resource/sound/select.mp3");
	ChangeVolumeSoundMem(120, selectSE);
	int cursorSE = LoadSoundMem("resource/sound/cursor.mp3");
	ChangeVolumeSoundMem(120, cursorSE);

	int titleGH[2];
	LoadDivGraph("resource/pict/title.png", 2, 2, 1, 1280, 800, titleGH);

	// ゲームループで使う変数の宣言
	int scene = TITLE;
	int stage = TUTORIAL;

	const int gameTime = 60 * 100;
	int gameTimer = gameTime;

	bool reset = true;

	Player player;
	Screen screen;
	Score score;
	Enemy enemy;


	bool sound = false;


	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	//マウス情報
	int mouseX;
	int mouseY;
	int mouse = 0;
	int oldMouse;

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; ++i) {
			oldkeys[i] = keys[i];
		}
		oldMouse = mouse;

		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		GetMousePoint(&mouseX, &mouseY);

		int mouse = GetMouseInput();

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		switch (scene) {
		case TITLE:
			StopSoundMem(scoreBGM);
			if (!CheckSoundMem(titleBGM)) PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);

			//ゲーム情報のリセット処理
			if (reset) {
				player.Reset(scene);
				gameTimer = gameTime;
				reset = false;
				stage = TUTORIAL;
			}

			//ステージ選択
			//上
			if (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP]) {
				stage--;
				if (stage < TUTORIAL) stage = GAME;
				PlaySoundMem(cursorSE, DX_PLAYTYPE_BACK, true);
			}

			//下
			if (keys[KEY_INPUT_DOWN] && !oldkeys[KEY_INPUT_DOWN]) {
				stage++;
				if (stage > GAME) stage = TUTORIAL;
				PlaySoundMem(cursorSE, DX_PLAYTYPE_BACK, true);
			}

			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				scene = LOAD;
				reset = true;
				/*if (stage == TUTORIAL) scene = TUTORIAL;
				else if (stage == GAME) {
					reset = true;
					scene = GAME;
				}*/
				PlaySoundMem(selectSE, DX_PLAYTYPE_NORMAL, true);
			}

			//描画
			if (stage == TUTORIAL) DrawGraph(0, 0, titleGH[0], true);
			else DrawGraph(0, 0, titleGH[1], true);

			/*DrawFormatString(0, 0, 0xffffff, "こびとのそうじめいじん");
			if (stage == TUTORIAL)DrawFormatString(0, 15, 0xffffff, "チュートリアル");
			else if (stage == GAME) DrawFormatString(0, 15, 0xffffff, "ゲーム");
			DrawFormatString(0, 30, 0xffffff, "↑↓で移動 : spaceで決定");*/
			break;
		case TUTORIAL:
			if (!CheckSoundMem(tutorialBGM)) PlaySoundMem(tutorialBGM, DX_PLAYTYPE_LOOP, true);
			//Tキーを押したらgameスタート
			if (keys[KEY_INPUT_T] && !oldkeys[KEY_INPUT_T]) {
				scene = LOAD;
				stage = GAME;
				reset = true;
			}

			screen.Background(player.scrollX, player.scrollY);
			
			player.Tutorial(keys, oldkeys, screen.tutorialScene);

			player.Sound(keys, oldkeys);

			screen.ScoreDraw(player.score);

			screen.TutorialAniGH(keys, oldkeys);


			break;
		case GAME:

			if (!CheckSoundMem(stageBGM)) PlaySoundMem(stageBGM, DX_PLAYTYPE_LOOP, true);

			if (reset) {
				player.Reset(scene);
				reset = false;
			}
			

			gameTimer--;
			if (gameTimer < 0) scene = SCORE;

			//描画
			screen.Background(player.scrollX, player.scrollY); 
			
			player.Option(keys, oldkeys, scene);

			player.Draw(keys, oldkeys);

			screen.ScoreDraw(player.score);

			screen.TimerDraw(gameTimer);

			player.Sound(keys, oldkeys);

			break;
		case SCORE:
			StopSoundMem(stageBGM);
			player.SoundStop();
			if (!CheckSoundMem(scoreBGM) && !sound) {
				PlaySoundMem(scoreBGM, DX_PLAYTYPE_BACK, true);
				sound = true;
			}
			//ゲームが終了したらリセットフラグを立てる
			score.Ranking(player.score);

			reset = true;
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				scene = TITLE;
				sound = false;
			}

			/*DrawFormatString(0, 0, 0xffffff, "score");
			DrawFormatString(0, 15, 0xffffff, "spaceでタイトル");*/
			break;
		case LOAD:
			StopSoundMem(titleBGM);
			StopSoundMem(tutorialBGM);
			player.SoundStop();
			screen.Load(scene, stage);
		}

		//確認
		//DrawFormatString(0, 0, 0xffffff, "%d", mouseX);

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
