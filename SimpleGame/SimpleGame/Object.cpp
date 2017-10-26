#include "stdafx.h"
#include "Object.h"



Object::Object()
{
}

Object::Object(float x, float y, float moveX, float moveY) : x(x), y(y), moveX(moveX), moveY(moveY)
{
	lifeCount = 10;
	timeCount = 0;
	death = false;
}

Object::~Object()
{
}

void Object::Move() {
	x += moveX;
	y += moveY;
}

void Object::Set(float setX, float setY) {
	x = setX;
	y = setY;
}

void Object::SetMoveX(float inputMoveY) { moveX = inputMoveY; }
void Object::SetMoveY(float inputMoveY) { moveY = inputMoveY; }
void Object::SetCollision(bool coll) { collision = coll; }

float Object::GetMoveX() { return moveX; }
float Object::GetMoveY() { return moveY; }
float Object::GetX() { return x; }
float Object::GetY() { return y; }
bool Object::GetCollision() { return collision; }
bool Object::GetDeath() { return death; }

void Object::minusLife() { 
	if (lifeCount < 1)
		death = true;
	else
		lifeCount -= 1;
	cout << lifeCount << endl;
}

void Object::update(DWORD elapsedTime)
{
	timeCount += elapsedTime;
	cout << elapsedTime << endl;
	if (timeCount > 1000000) {
		timeCount - 1000000;
		if (death == false)
			minusLife();
	}
}

