#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::Object(float x, float y, int ObjectType) : x(x), y(y)
{
	if (ObjectType == 0) { life = 500, speed = 0, size = 50, r = 120, g = 120, b = 0; }			// 건물
	else if (ObjectType == 1){ life = 10, speed = 300, size = 10, r = 255, g = 255, b = 255; }	// 플레이어
	else if (ObjectType == 2){ life = 20, speed = 600, size = 2, r = 255, g = 0, b = 0; }		// 총알
	else if (ObjectType == 3){ life = 10, speed = 100, size = 2, r = 0, g = 255, b = 0;	}		// 화살
	
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

void Object::update(DWORD elapsedTime)
{

	float elapsedTimeInSecond = elapsedTime / 1000.f;

	if (death == true)
		x = 500, y = 500;
	else 
	{
		x += moveX * elapsedTimeInSecond * speed / 100;
		y += moveY * elapsedTimeInSecond * speed / 100;
	}
	if (lifeTime > 0)
		lifeTime -= elapsedTimeInSecond;
	else
		death = true;
	
	
}

