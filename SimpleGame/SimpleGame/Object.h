#pragma once
class Object
{
private:
	float x, y;
	float moveX, moveY;
	bool collision;

	bool death;
	int life;
	int speed;
	int size;
	int r;
	int g;
	int b;
	float lifeTime;

	DWORD timeCount;

public:
	Object();
	Object(float x, float y, int ObjectType);
	~Object();

	void SetPosition(float setX, float setY);
	void SetMoveX(float moveX);
	void SetMoveY(float moveY);
	void SetCollision(bool coll);
	void SetDeath(bool death);
	void SetLife(int inputLife);

	float GetMoveX();
	float GetMoveY();
	float GetX();
	float GetY();
	int	  GetSize();
	int	  GetR();
	int	  GetG();
	int	  GetB();
	int   GetLife();
	bool  GetCollision();
	bool  GetDeath();

	void minusLife(Object* minusObject);

	void update(DWORD elapsedTime);

};

