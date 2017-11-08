#include "stdafx.h"
#include "SceneMgr.h"


default_random_engine dre;
uniform_int_distribution<int> ranPos(-250, 250);

Renderer *renderer = NULL;

SceneMgr::SceneMgr()
{
	objectCount = 0;
	bulletCount = 0;
}

SceneMgr::~SceneMgr()
{
}

void SceneMgr::Release() 
{
	dre.seed(time(NULL));

	renderer = new Renderer(500, 500);

	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	building = new Object(0, 0, OBJECT_BUILDING);
	
}

void SceneMgr::AddObject(float x, float y, int objectType) 
{
	if (objectCount < CHARACTER_MAX_COUNT) {
		object[objectCount] = new Object(x, y, objectType);
		++objectCount;
	}
	else
	{
		int i = 0;
		while (i < CHARACTER_MAX_COUNT) {
			if (object[i]->GetDeath() == true) {
				object[i]->SetPosition(x, y);
				object[i]->SetDeath(false);
				object[i]->SetLife(10);
				break;
			}
			++i;
		}
	}
			
}

void SceneMgr::AddBullet()
{
	if (bulletCount < BULLET_MAX_COUNT) {
		bullet[bulletCount] = new Object(0, 0, OBJECT_BULLET);
		++bulletCount;
	}

	bulletTime = 0.5;
}

void SceneMgr::wallCheck(Object* object) {
	// 벽과 충돌체크
	if (object->GetX() < -250 || object->GetX() > 250)
		object->SetMoveX((-1)*object->GetMoveX());
	if (object->GetY() < -250 || object->GetY() > 250)
		object->SetMoveY((-1)*object->GetMoveY());
}

bool SceneMgr::Collision(Object* mainObj, Object* collObj)
{
	// 캐릭터간 충돌체크
	if (mainObj->GetX() - collObj->GetX() < (collObj->GetSize() + mainObj->GetSize())/2 && mainObj->GetX() - collObj->GetX() > -(collObj->GetSize() + mainObj->GetSize()) / 2
		&& mainObj->GetY() - collObj->GetY() < (collObj->GetSize() + mainObj->GetSize()) / 2 && mainObj->GetY() - collObj->GetY() > -(collObj->GetSize() + mainObj->GetSize()) / 2) {
		mainObj->SetCollision(true);
		return true;
	}

	return 0;
}

void SceneMgr::Darw() 
{
	if (building->GetDeath() == false) {
		if (building->GetCollision() == true)
			renderer->DrawSolidRect(building->GetX(), building->GetY(), 0, building->GetSize(), 1, 0, 0, 1);
		else
			renderer->DrawSolidRect(building->GetX(), building->GetY(), 0, building->GetSize(), building->GetR(), building->GetG(), building->GetB(), 1);
		building->SetCollision(false);
	}
	for (int i = 0; i < objectCount; ++i) 
	{
		if (object[i]->GetCollision() == true)
			renderer->DrawSolidRect(object[i]->GetX(), object[i]->GetY(), 0, object[i]->GetSize(), 1, 0, 0, 1);
		else
			renderer->DrawSolidRect(object[i]->GetX(), object[i]->GetY(), 0, object[i]->GetSize(), object[i]->GetR(), object[i]->GetG(), object[i]->GetB(), 1);
		object[i]->SetCollision(false);
	}

	for (int i = 0; i < bulletCount; ++i) {
		renderer->DrawSolidRect(bullet[i]->GetX(), bullet[i]->GetY(), 0, bullet[i]->GetSize(), bullet[i]->GetR(), bullet[i]->GetG(), bullet[i]->GetB(), 2);
	}
}

void SceneMgr::Update(DWORD elapsedTime) 
{

	for (int i = 0; i < objectCount; ++i)				// 캐릭터 - 총알 충돌
	{
		wallCheck(object[i]);
		if (object[i]->GetDeath() == false)
		{
			for (int j = 0; j < bulletCount; ++j)
			{
				if (Collision(object[i], bullet[j])) {
					object[i]->minusLife(bullet[j]);
					bullet[j]->SetDeath(true);
				}
			}
		}
	}


	if (building->GetDeath() == false)					// 건물 - 캐릭터 충돌
	{
		for (int i = 0; i < objectCount; ++i)
		{
			if (Collision(building, object[i])) {
				building->minusLife(object[i]);
				object[i]->SetDeath(true);
				cout << building->GetLife() << endl;
			}
		}
	}

	for (int i = 0; i < bulletCount; ++i)
		wallCheck(bullet[i]);

	for (int i = 0; i < objectCount; ++i) 
	{
		if (!object[i]->GetDeath()) 
		{
			object[i]->update(elapsedTime);
		}
		if (object[i]->GetDeath())
			object[i]->SetPosition(1000, 1000);
	}	

	for (int i = 0; i < bulletCount; ++i) {
		if (!bullet[i]->GetDeath())
		{
			bullet[i]->update(elapsedTime);
		}
		if (bullet[i]->GetDeath())
			bullet[i]->SetPosition(500, 500);
	}

	float elapsedTimeInSecond = elapsedTime / 1000.f;
	if (bulletTime > 0)
		bulletTime -= elapsedTimeInSecond;
	else
		AddBullet();

}
