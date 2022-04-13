#include "DxLib.h"
#include "enemy.h"
#include"screen.h"
#include"enum.h"

Enemy::Enemy() {

	//�G
//�l��
	human.x = WOR_WIDTH;
	human.y = WOR_HEIGHT - 768;
	human.width = 384;
	human.height = 768;
	human.speed = -4;
	humanFlag = false;
	LoadDivGraph("resource/pict/enemy/girlleft.png", 4, 4, 1, 384, 768, humanLeftGH);
	LoadDivGraph("resource/pict/enemy/girlright.png", 4, 4, 1, 384, 768, humanRightGH);
	humanNum = 0;
	humanDirectFlag = false;


	//�����o
	roomba.x = WOR_WIDTH;
	roomba.y = WOR_HEIGHT - 64;
	roomba.width = 256;
	roomba.height = 64;
	roomba.speed = -3;
	roombaFlag = false;
	LoadDivGraph("resource/pict/enemy/rumba.png", 2, 2, 1, 256, 64, roombaGH);
	roombaDirectFlag = false;

	object = new Obj;

	//�G���o�Ă���^�C�~���O�̕ϐ�
	randomNumber = 0;
	randomTimer = 100;
	coolTimer = 900;
	enemyFlag = false;

	//������͈͂̕ϐ�
	a.x = 0;
	a.y = 0;
	b.x = 0;
	b.y = 0;
	c.x = 0;
	c.y = 0;
	size = WIN_WIDTH / 2;
	hitFlag = false;
	left_up_flag = false;
	right_up_flag = false;
	left_down_flag = false;
	right_down_flag = false;

	keikoku.sizeX = 128;
	keikoku.sizeY = 128;
	keikoku.x = WIN_WIDTH - keikoku.sizeX;
	keikoku.y = 400 - keikoku.sizeY / 2;
	keikokuGH = LoadGraph("resource/pict/keikoku.png");

}

Enemy::~Enemy()//�����ׂ����킩��񂩂�Ƃ肠��������Ă�
{
	if (object != NULL) delete object;
}


//�G���o�Ă���֐�
void Enemy::EnemyTiming(float scrollX, float scrollY) {
	randomTimer--;
	if (randomTimer == 90)
	{
		randomNumber = GetRand(9) + 1;
	}
	if (randomTimer < 0)
	{
		if (randomNumber > 0 && randomNumber < 4)//�l��
		{
			HumanMove();
			humanFlag = true;
		}
		else if (randomNumber > 3 && randomNumber < 11)//�����o
		{
			RoombaMove();
			roombaFlag = true;
		}

		if (human.x > WOR_WIDTH + 10 || roomba.x > WOR_WIDTH + 10)
		{
			randomNumber = 0;
			randomTimer = 480;
			human.x = WOR_WIDTH;
			human.speed = -4;
			roomba.x = WOR_WIDTH;
			roomba.speed = -3;
			humanFlag = false;
			roombaFlag = false;
			humanDirectFlag = false;
			roombaDirectFlag = false;
		}

	}
}

void Enemy::Warning() {
	if(randomNumber > 0 && randomNumber < 4 && randomTimer >= 0)
	DrawGraph(keikoku.x, keikoku.y, keikokuGH, true);
	//DrawFormatString(0, 0, 0x000000, "%d", randomTimer);
}


//�l�Ԃ̓����ƕ`��
void Enemy::HumanMove() {
	if (humanFlag == true)
	{
		human.x += human.speed;
		if (human.x < 0)
		{
			human.speed = -human.speed;
			humanDirectFlag = true;
		}
	}
}
void Enemy::HumanDraw(float scrollX, float scrollY) {
	if (humanFlag == true)
	{
		//�A�j���[�V����
		int j = humanMove.GHTimer / (humanMove.GHTime / humanMove.GHNum);
		static int GH;
		int add = 0;

		humanMove.GHTimer++;
		if (humanDirectFlag == true)//�E����
		{
			GH = humanRightGH[j];
		}
		else
		{
			GH = humanLeftGH[j];
		}
		//�A�j���[�V�����^�C�}�[���Z�b�g
		if (humanMove.GHTimer >= humanMove.GHTime) humanMove.GHTimer = 0;
		//�l�ԕ`��
		DrawGraph(human.x - scrollX, human.y - scrollY, GH, true);
	}
}


//�����o�����ƕ`��
void Enemy::RoombaMove() {
	if (roombaFlag == true)
	{
		roomba.x += roomba.speed;
		if (roomba.x < 0)
		{
			roomba.speed = -roomba.speed;
			roombaDirectFlag = true;
		}
	}
}

void Enemy::RoombaDraw(float scrollX, float scrollY) {
	if (roombaFlag == true)
	{
		if (roombaDirectFlag == false)DrawGraph(roomba.x - scrollX, roomba.y - scrollY, roombaGH[0], true);//������
		else DrawGraph(roomba.x - scrollX, roomba.y - scrollY, roombaGH[1], true);//�E����
	}
}







//������͈�
void Enemy::HumanSearchCollision(float player_x, float player_y, float player_width, float player_height, float scrollX, float scrollY, bool hideFlag)//+�����͂��꒲��
{
	if (humanFlag == true)
	{
		a.y = human.y + 191;
		b.y = WOR_HEIGHT;
		c.y = WOR_HEIGHT;
		//������
		if (humanDirectFlag == false)
		{
			a.x = human.x + 40;
			b.x = a.x - size / 6;
			c.x = a.x - size;
		}
		else//�E����
		{
			a.x = human.x + human.width - 40;
			b.x = a.x + size;
			c.x = a.x + size / 6;
		}

		points[0] = { a.x,a.y };
		points[1] = { b.x,b.y };
		points[2] = { c.x,c.y };

		//�v���C���[�̏��
		//����
		float left_up_x = player_x;
		float left_up_y = player_y;
		//�E��
		float right_up_x = player_x + player_width;
		float right_up_y = player_y;
		//����
		float left_down_x = player_x;
		float left_down_y = player_y + player_height;
		//�E��
		float right_down_x = player_x + player_width;
		float right_down_y = player_y + player_height;

		//�����蔻�菈��
		if (hideFlag == false)
		{
			//�v���C���[�̍���
			for (int i = 0; i < 3; i++)
			{
				Vec2& s1 = points[i];
				Vec2& e1 = points[(i + 1) % 3];

				Vec2 a1;
				a1.x = e1.x - s1.x;
				a1.y = e1.y - s1.y;

				Vec2 b1;
				b1.x = left_up_x - s1.x;
				b1.y = left_up_y - s1.y;

				if ((a1.x * b1.y - a1.y * b1.x) >= 0)
				{
					left_up_flag = true;
				}
				else
				{
					left_up_flag = false;
					break;
				}
			}

			//�v���C���[�̉E��
			for (int i = 0; i < 3; i++)
			{
				Vec2& s2 = points[i];
				Vec2& e2 = points[(i + 1) % 3];

				Vec2 a2;
				a2.x = e2.x - s2.x;
				a2.y = e2.y - s2.y;

				Vec2 b2;
				b2.x = right_up_x - s2.x;
				b2.y = right_up_y - s2.y;

				if ((a2.x * b2.y - a2.y * b2.x) >= 0)
				{
					right_up_flag = true;
				}
				else
				{
					right_up_flag = false;
					break;
				}
			}
			//�v���C���[�̍���
			for (int i = 0; i < 3; i++)
			{
				Vec2& s3 = points[i];
				Vec2& e3 = points[(i + 1) % 3];

				Vec2 a3;
				a3.x = e3.x - s3.x;
				a3.y = e3.y - s3.y;

				Vec2 b3;
				b3.x = left_down_x - s3.x;
				b3.y = left_down_y - s3.y;

				if ((a3.x * b3.y - a3.y * b3.x) >= 0)
				{
					left_down_flag = true;
				}
				else
				{
					left_down_flag = false;
					break;
				}
			}
			//�v���C���[�̉E��
			for (int i = 0; i < 3; i++)
			{
				Vec2& s4 = points[i];
				Vec2& e4 = points[(i + 1) % 3];

				Vec2 a4;
				a4.x = e4.x - s4.x;
				a4.y = e4.y - s4.y;

				Vec2 b4;
				b4.x = right_down_x - s4.x;
				b4.y = right_down_y - s4.y;

				if ((a4.x * b4.y - a4.y * b4.x) >= 0)
				{
					right_down_flag = true;
				}
				else
				{
					right_down_flag = false;
					break;
				}
			}

			//�`��
			if ((left_up_flag == true || right_up_flag == true || left_down_flag == true || right_down_flag == true) && hideFlag == false)
			{
				hitFlag = true;
			}
			else hitFlag = false;
		}
		for (int i = 0; i < 3; i++)
		{
			Vec2& s = points[i];
			Vec2& e = points[(i + 1) % 3];
			DrawTriangle(a.x - scrollX, a.y - scrollY, b.x - scrollX, b.y - scrollY, c.x - scrollX, c.y - scrollY, 0xFF0000, true);
		}
	}
}//�����ɓ����蔻��ǉ��\��
