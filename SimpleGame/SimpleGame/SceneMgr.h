#pragma once
#include "Object.h"
#include "Renderer.h"

#define OBJECT_BUILDING		0
#define OBJECT_CHARACTER	1
#define OBJECT_BULLET		2
#define OBJECT_ARROW		3

#define OBJECT_MAX_COUNT	50
#define BUILDING_MAX_COUNT	1
#define CHARACTER_MAX_COUNT 10
#define BULLET_MAX_COUNT	50
#define ARROW_MAX_COUNT		3

class SceneMgr
{
private:
	int objectCount;
	int bulletCount;
	float bulletTime;
	int arrowCount[10];
	float arrowTime[10];

public:
	SceneMgr();
	~SceneMgr();

	Object* building;
	Object* bullet[100];
	Object* object[10];		// ĳ����
	Object* arrow[10][100];

	void AddObject(float x, float y, int objectType);
	void AddBullet();
	bool Collision(Object* mainObj, Object* collObj);
	void wallCheck(Object* object);
	void Release();
	void Darw();
	void Update(DWORD elapsedTime);
};

