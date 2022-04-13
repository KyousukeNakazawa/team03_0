#include "DxLib.h"
#include "screen.h"
#include "player.h"
#include "enum.h"
#include "score.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char WIN_TITLE[] = "���тƂ̂������߂�����";

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(WIN_TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0xff, 0xff, 0xff);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
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

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
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


	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	//�}�E�X���
	int mouseX;
	int mouseY;
	int mouse = 0;
	int oldMouse;

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; ++i) {
			oldkeys[i] = keys[i];
		}
		oldMouse = mouse;

		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		GetMousePoint(&mouseX, &mouseY);

		int mouse = GetMouseInput();

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		switch (scene) {
		case TITLE:
			StopSoundMem(scoreBGM);
			if (!CheckSoundMem(titleBGM)) PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);

			//�Q�[�����̃��Z�b�g����
			if (reset) {
				player.Reset(scene);
				gameTimer = gameTime;
				reset = false;
				stage = TUTORIAL;
			}

			//�X�e�[�W�I��
			//��
			if (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP]) {
				stage--;
				if (stage < TUTORIAL) stage = GAME;
				PlaySoundMem(cursorSE, DX_PLAYTYPE_BACK, true);
			}

			//��
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

			//�`��
			if (stage == TUTORIAL) DrawGraph(0, 0, titleGH[0], true);
			else DrawGraph(0, 0, titleGH[1], true);

			/*DrawFormatString(0, 0, 0xffffff, "���тƂ̂������߂�����");
			if (stage == TUTORIAL)DrawFormatString(0, 15, 0xffffff, "�`���[�g���A��");
			else if (stage == GAME) DrawFormatString(0, 15, 0xffffff, "�Q�[��");
			DrawFormatString(0, 30, 0xffffff, "�����ňړ� : space�Ō���");*/
			break;
		case TUTORIAL:
			if (!CheckSoundMem(tutorialBGM)) PlaySoundMem(tutorialBGM, DX_PLAYTYPE_LOOP, true);
			//T�L�[����������game�X�^�[�g
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

			//�`��
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
			//�Q�[�����I�������烊�Z�b�g�t���O�𗧂Ă�
			score.Ranking(player.score);

			reset = true;
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				scene = TITLE;
				sound = false;
			}

			/*DrawFormatString(0, 0, 0xffffff, "score");
			DrawFormatString(0, 15, 0xffffff, "space�Ń^�C�g��");*/
			break;
		case LOAD:
			StopSoundMem(titleBGM);
			StopSoundMem(tutorialBGM);
			player.SoundStop();
			screen.Load(scene, stage);
		}

		//�m�F
		//DrawFormatString(0, 0, 0xffffff, "%d", mouseX);

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
