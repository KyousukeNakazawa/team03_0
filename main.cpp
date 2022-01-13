#include "DxLib.h"
#include "screen.h"
#include "player.h"
#include "enum.h"
#include "score.h"
#include <stdio.h>
#include <stdlib.h>

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char WIN_TITLE[] = "";

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
	SetBackgroundColor(0x00, 0x00, 0xff);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int scene = TITLE;
	int stage = TUTORIAL;

	const int gameTime = 60 * 100;
	int gameTimer = gameTime;

	bool reset = true;

	Player player;
	Screen screen;
	Score score;


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
			//�Q�[�����̃��Z�b�g����
			if (reset) {
				player.Reset();
				gameTimer = gameTime;
				reset = false;
				stage = TUTORIAL;
			}

			//�X�e�[�W�I��
			//��
			if (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP]) {
				stage--;
				if (stage <= TUTORIAL) stage = TUTORIAL;
			}

			//��
			if (keys[KEY_INPUT_DOWN] && !oldkeys[KEY_INPUT_DOWN]) {
				stage++;
				if (stage >= GAME) stage = GAME;
			}

			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				if (stage == TUTORIAL) scene = TUTORIAL;
				else if (stage == GAME) scene = GAME;
			}

			DrawFormatString(0, 0, 0xffffff, "title");
			DrawFormatString(0, 15, 0xffffff, "%d", stage);
			break;
		case TUTORIAL:

			break;
		case GAME:
			player.Option(keys, oldkeys, mouse, oldMouse, mouseX, mouseY);

			gameTimer--;
			if (gameTimer < 0) scene = SCORE;

			//�`��
			screen.Background(player.scrollX, player.scrollY);

			player.Draw(keys, oldkeys, mouse, oldMouse, mouseX, mouseY);

			screen.ScoreDraw(player.score);

			screen.TimerDraw(gameTimer);

			break;
		case SCORE:
			//�Q�[�����I�������烊�Z�b�g�t���O�𗧂Ă�
			score.Ranking(player.score);

			reset = true;
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) scene = TITLE;

			DrawFormatString(0, 0, 0xffffff, "score");
			break;
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
