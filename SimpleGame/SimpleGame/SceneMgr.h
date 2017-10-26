#pragma once
#include "Object.h"
#include "Renderer.h"

class SceneMgr
{
private:
	int objectCount;

public:
	SceneMgr();
	~SceneMgr();

	Object* m_object[50];

	void AddObject(float x, float y, float moveX, float moveY);
	void Collision(int inputNum);
	void Release();
	void Delete();
	void Darw();
	void Update(DWORD elapsedTime);
};

