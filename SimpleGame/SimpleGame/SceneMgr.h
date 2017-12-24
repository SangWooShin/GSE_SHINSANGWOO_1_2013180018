#pragma once
#include "Object.h"
#include "Renderer.h"
#include "Sound.h"

#define OBJECT_BUILDING		0
#define OBJECT_CHARACTER_0	1
#define OBJECT_CHARACTER_1	2
#define OBJECT_BULLET_0		3
#define OBJECT_BULLET_1		4
#define OBJECT_ARROW_0		5
#define OBJECT_ARROW_1		6

#define OBJECT_MAX_COUNT	50
#define BUILDING_MAX_COUNT	3
#define CHARACTER_MAX_COUNT 10
#define BULLET_MAX_COUNT	99
#define ARROW_MAX_COUNT		3

#define WINDOW_WIDTH_MAX	500
#define WINDOW_HIGHT_MAX	800

class SceneMgr
{
private:
	int		objectCount[2];
	int		bulletCount[2];
	float	bulletTime;
	int		arrowCount [2][10];
	float	arrowTime[2][10];
	float	objectTime;
	float	objectCoolTime;
	float	particleTime;
	int		sceneTransform = 0;
	int		animateCount[2];

	Sound* m_sound;
	int soundBG;

	GLuint texCharacter[6];
	GLuint texGrass;

public:
	SceneMgr();
	~SceneMgr();

	Object* building[2][3];
	Object* bullet[2][99];
	Object* object[2][10];		// Ä³¸¯ÅÍ
	Object* arrow[2][10][10];

	void AddObject(float x, float y, int objectType, int team);
	void AddBullet();
	void AddArrow(int i, int j);
	bool Collision(Object* mainObj, Object* collObj);
	void wallCheck(Object* object);
	void Release();
	void Darw(float elapsedTimeInSecond);
	void Update(float elapsedTime);
	void SceneTransform();
};

