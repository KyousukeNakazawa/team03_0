#include "DxLib.h"
#include "enemy.h"
#include"screen.h"
#include"enum.h"

Enemy::Enemy() {
	human.x = WIN_WIDTH * 1.5;
	human.y = WIN_HEIGHT - 768;
	human.width = 384;
	human.height = 768;
	human.speed = -5;

	rumba.x = WIN_WIDTH * 1.5;
	rumba.y = WIN_HEIGHT - 64;
	rumba.width = 256;
	rumba.height = 64;
	rumba.speed = -5;

	cat.x = WIN_WIDTH * 1.5;
	cat.y = WIN_HEIGHT - 256;
	cat.width = 512;
	cat.height = 256;
	cat.speed = -5;

	dog.x = WIN_WIDTH * 1.5;
	dog.y = WIN_HEIGHT - 256;
	dog.width = 512;
	dog.height = 256;
	dog.speed = -5;
}

void Enemy::HumanMove() {
	human.x += human.speed;
	if (human.x < 0)
	{
		human.speed = -human.speed;
	}
	if (human.x > WIN_WIDTH * 3 - human.width)
	{
		human.speed = -human.speed;
	}
}
void Enemy::HumanDraw(int scroll) {
	DrawBox(human.x - scroll, human.y, human.x + human.width - scroll, human.y + human.height, GetColor(255, 255, 255), true);
}

void Enemy::RumbaMove() {
	rumba.x += rumba.speed;
	if (rumba.x < 0)
	{
		rumba.speed = -rumba.speed;
	}
	if (rumba.x > WIN_WIDTH * 3 - rumba.width)
	{
		rumba.speed = -rumba.speed;
	}
}
void Enemy::RumbaDraw(int scroll) {
	DrawBox(rumba.x - scroll, rumba.y, rumba.x + rumba.width - scroll, rumba.y + rumba.height, GetColor(255, 255, 255), true);
}

void Enemy::CatMove() {
	cat.x += cat.speed;
	if (cat.x < 0)
	{
		cat.speed = -cat.speed;
	}
	if (cat.x > WIN_WIDTH * 3 - cat.width)
	{
		cat.speed = -cat.speed;
	}
}
void Enemy::CatDraw(int scroll) {
	DrawBox(cat.x - scroll, cat.y, cat.x + cat.width - scroll, cat.y + cat.height, GetColor(255, 255, 255), true);
}

void Enemy::DogMove() {
	dog.x += dog.speed;
	if (dog.x < 0)
	{
		dog.speed = -dog.speed;
	}
	if (dog.x > WIN_WIDTH * 3 - dog.width)
	{
		dog.speed = -dog.speed;
	}
}
void Enemy::DogDraw(int scroll) {
	DrawBox(dog.x - scroll, dog.y, dog.x + dog.width - scroll, dog.y + dog.height, GetColor(255, 255, 255), true);
}