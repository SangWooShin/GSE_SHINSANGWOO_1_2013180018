#pragma once
class Object
{
private:
	float x, y;
	float moveX, moveY;
	bool collision;
	bool death;
	int lifeCount;
	DWORD timeCount;

public:
	Object();
	Object(float x, float y, float moveX, float moveY);
	~Object();

	void Move();

	void Set(float setX, float setY);
	void SetMoveX(float moveX);
	void SetMoveY(float moveY);
	void SetCollision(bool coll);


	float GetMoveX();
	float GetMoveY();
	float GetX();
	float GetY();
	bool GetCollision();
	bool GetDeath();

	void minusLife();

	void update(DWORD elapsedTime);

};

