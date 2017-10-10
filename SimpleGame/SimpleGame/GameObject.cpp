#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::GameObject(float x, float y) : x(x), y(y)
{
}

void GameObject::Move(float moveX, float moveY) {
	x += moveX;
	y += moveY;
}

void GameObject::Set(float setX, float setY) {
	x = setX;
	y = setY;
}

float GameObject::GetX() { return x; }
float GameObject::GetY() { return y; }

GameObject::~GameObject()
{
}
