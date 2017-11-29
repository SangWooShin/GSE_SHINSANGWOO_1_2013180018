#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::Object(float x, float y, int ObjectType) : x(x), y(y)
{
	if (ObjectType == 0) { life = 500, speed = 0, size = 100, r = 1, g = 1, b = 0; }		// 건물
	else if (ObjectType == 1){ life = 100, speed = 300, size = 10, r = 1, g = 0, b = 0; }	// 플레이어_0
	else if (ObjectType == 2) { life = 100, speed = 300, size = 10, r = 0, g = 0, b = 1; }	// 플레이어
	else if (ObjectType == 3){ life = 20, speed = 600, size = 5, r = 1, g = 0, b = 0; }		// 총알_0
	else if (ObjectType == 4){ life = 20, speed = 600, size = 5, r = 0, g = 0, b = 1; }		// 총알_1
	else if (ObjectType == 5){ life = 10, speed = 100, size = 2, r = 1.0, g = 0.7, b = 1.0;	}// 화살_0
	else if (ObjectType == 6){ life = 10, speed = 100, size = 2, r = 1, g = 1, b = 0; }		// 화살_1
	
	death = 0;
	lifeTime = 1000.f;
	moveX = 200.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
	moveY = 200.f *(((float)std::rand() / (float)RAND_MAX) - 0.5f);
}

Object::~Object()
{
}

void Object::SetPosition(float setX, float setY)	{ x = setX, y = setY; }
void Object::SetMoveX(float inputMoveY)				{ moveX = inputMoveY; }
void Object::SetMoveY(float inputMoveY)				{ moveY = inputMoveY; }
void Object::SetCollision(bool coll)				{ collision = coll; }
void Object::SetDeath(bool inputdeath)				{ death = inputdeath; }
void Object::SetLife(int inputLife)					{ life = inputLife; }

float Object::GetMoveX()		{ return moveX; }
float Object::GetMoveY()		{ return moveY; }
float Object::GetX()			{ return x; }
float Object::GetY()			{ return y; }
int	  Object::GetSize()			{ return size; }
int	  Object::GetR()			{ return r; }
int	  Object::GetG()			{ return g; }
int	  Object::GetB()			{ return b; }
int   Object::GetLife()			{ return life; }
bool  Object::GetCollision()	{ return collision; }
bool  Object::GetDeath()		{ return death; }

void Object::minusLife(Object* minusObject) { 
	life -= minusObject->GetLife();
	if (life < 0)
		death = true;
}

void Object::update(float elapsedTimeInSecond)
{
	if (death == true)
		x = 1000, y = 1000;
	else 
	{
		x += moveX * elapsedTimeInSecond * speed / 50;
		y += moveY * elapsedTimeInSecond * speed / 50;
	}
	if (lifeTime > 0)
		lifeTime -= elapsedTimeInSecond;
	else
		death = true;
}

