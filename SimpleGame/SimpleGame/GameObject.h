#pragma once
class GameObject
{
private:
	float x, y;
public:
	GameObject();
	GameObject(float x, float y);
	void Move(float moveX, float moveY);
	void Set(float setX, float setY);
	float GetX();
	float GetY();
	~GameObject();
	
};

