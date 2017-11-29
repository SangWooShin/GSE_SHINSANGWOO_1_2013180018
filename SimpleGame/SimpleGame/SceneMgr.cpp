#include "stdafx.h"
#include "SceneMgr.h"


default_random_engine dre;
uniform_int_distribution<int> ranPos(-250, 250);

Renderer *renderer = NULL;

SceneMgr::SceneMgr()
{
	objectCount[0] = 0;
	objectCount[1] = 0;
	bulletCount[0] = 0;
	bulletCount[1] = 0;

	texCharacter[0] = renderer->CreatePngTexture("../Resource/ohhh.png");
	texCharacter[1] = renderer->CreatePngTexture("../Resource/ohh.png");
	
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 10; ++j) {
			arrowCount[i][j] = 0;
			arrowTime[i][j] = 0;
		}
	}

	objectCoolTime = 7;
}

SceneMgr::~SceneMgr()
{
}
 
void SceneMgr::Release() 
{
	dre.seed(time(NULL));

	renderer = new Renderer(500, 800);

	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < BUILDING_MAX_COUNT; ++j)
			building[i][j] = new Object(-150 + 150 * j, -600 * i + 300, OBJECT_BUILDING);
	}
}

void SceneMgr::AddObject(float x, float y, int objectType, int team) 
{
	if (objectCoolTime > 7 || team == 0) {
		if (objectCount[team] < CHARACTER_MAX_COUNT) {
			object[team][objectCount[team]] = new Object(x, y, objectType);
			++objectCount[team];
		}
		else
		{
			int i = 0;
			while (i < CHARACTER_MAX_COUNT) {
				if (object[team][i]->GetDeath() == true) {
					object[team][i]->SetPosition(x, y);
					object[team][i]->SetDeath(false);
					object[team][i]->SetLife(10);
					break;
				}
				++i;
			}
		}
		objectTime = 5;
		if(team == 1)
			objectCoolTime = 0;
	}
}

void SceneMgr::AddBullet()
{
	for (int i = 0; i < 2; ++i) {
		if (bulletCount[i] < BULLET_MAX_COUNT) {	
			for (int j = 0; j < BUILDING_MAX_COUNT; ++j) {
				if(i==0)
					bullet[i][bulletCount[i]] = new Object(-150 + 150 * j, -600 * i + 300, OBJECT_BULLET_0);
				else
					bullet[i][bulletCount[i]] = new Object(-150 + 150 * j, -600 * i + 300, OBJECT_BULLET_1);
				++bulletCount[i];
			}
		}
	}
	bulletTime = 10;
}

void SceneMgr::AddArrow(int i, int j) {
	if(i==0)
		arrow[i][j][arrowCount[i][j]] = new Object(object[i][j]->GetX(), object[i][j]->GetY(), OBJECT_ARROW_0);
	else
		arrow[i][j][arrowCount[i][j]] = new Object(object[i][j]->GetX(), object[i][j]->GetY(), OBJECT_ARROW_1);
	++arrowCount[i][j];
	arrowTime[i][j] = 3;
}


void SceneMgr::wallCheck(Object* object) {
	// 벽과 충돌체크
	if (object->GetX() < -((WINDOW_WIDTH_MAX / 2)) || object->GetX() > (WINDOW_WIDTH_MAX / 2))
		object->SetMoveX((-1)*object->GetMoveX());
	if (object->GetY() < -(WINDOW_HIGHT_MAX / 2) || object->GetY() > (WINDOW_HIGHT_MAX / 2))
		object->SetMoveY((-1)*object->GetMoveY());
}

bool SceneMgr::Collision(Object* mainObj, Object* collObj)
{
	// 충돌체크
	if (mainObj->GetX() - collObj->GetX() < (collObj->GetSize() + mainObj->GetSize())/2 && mainObj->GetX() - collObj->GetX() > -(collObj->GetSize() + mainObj->GetSize()) / 2
		&& mainObj->GetY() - collObj->GetY() < (collObj->GetSize() + mainObj->GetSize()) / 2 && mainObj->GetY() - collObj->GetY() > -(collObj->GetSize() + mainObj->GetSize()) / 2) {
		mainObj->SetCollision(true);
		return true;
	}

	return 0;
}

void SceneMgr::Darw() 
{
	

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < BUILDING_MAX_COUNT; ++j) {
				if (building[i][j]->GetDeath() == false) {
					renderer->DrawTexturedRect(building[i][j]->GetX(), building[i][j]->GetY(), 0, building[i][j]->GetSize(), 1, 1, 1, 1, texCharacter[i], 0.1);
					if(i==0)
						renderer->DrawSolidRectGauge(building[i][j]->GetX(), building[i][j]->GetY() + 60, 0, 90, 5, 1, 0, 0, 1, (float) building[i][j]->GetLife()/500,0.1);
					else
						renderer->DrawSolidRectGauge(building[i][j]->GetX(), building[i][j]->GetY() + 60, 0, 90, 5, 0, 0, 1, 1, (float)building[i][j]->GetLife() / 500, 0.1);
					building[i][j]->SetCollision(false);
				}
		}

		for (int j = 0; j < objectCount[i]; ++j) {
			renderer->DrawSolidRect(object[i][j]->GetX(), object[i][j]->GetY(), 0, object[i][j]->GetSize(), bullet[i][j]->GetR(), bullet[i][j]->GetG(), bullet[i][j]->GetB(), 1, 0.2);
			renderer->DrawSolidRectGauge(object[i][j]->GetX(), object[i][j]->GetY() + 10, 0, 10, 3, bullet[i][j]->GetR(), bullet[i][j]->GetG(), bullet[i][j]->GetB(), 1, (float)object[i][j]->GetLife() / 100, 0.2);
			object[i][j]->SetCollision(false);

			for (int h = 0; h < arrowCount[i][j]; ++h) {
				if(i==0)
					renderer->DrawSolidRect(arrow[i][j][h]->GetX(), arrow[i][j][h]->GetY(), 0, arrow[i][j][h]->GetSize(), arrow[i][j][h]->GetR(), arrow[i][j][h]->GetG(), arrow[i][j][h]->GetB(), 1, 0.3);
				else
					renderer->DrawSolidRect(arrow[i][j][h]->GetX(), arrow[i][j][h]->GetY(), 0, arrow[i][j][h]->GetSize(), arrow[i][j][h]->GetR(), arrow[i][j][h]->GetG(), arrow[i][j][h]->GetB(), 1, 0.3);
			}
		}

		for (int j = 0; j < bulletCount[i]; ++j) {
			if (i == 0)
				renderer->DrawSolidRect(bullet[i][j]->GetX(), bullet[i][j]->GetY(), 0, bullet[i][j]->GetSize(), bullet[i][j]->GetR(), bullet[i][j]->GetG(), bullet[i][j]->GetB(), 1, 0.3);
			else
				renderer->DrawSolidRect(bullet[i][j]->GetX(), bullet[i][j]->GetY(), 0, bullet[i][j]->GetSize(), bullet[i][j]->GetR(), bullet[i][j]->GetG(), bullet[i][j]->GetB(), 1, 0.3);
		}
	}
}

void SceneMgr::Update(float elapsedTimeInSecond)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//			충돌체크
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < objectCount[i]; ++j)				// 캐릭터 - 총알 충돌
		{
			wallCheck(object[i][j]);
			if (object[i][j]->GetDeath() == false)
			{
				int collCheck;
				if (i == 0)
					collCheck = 1;
				else
					collCheck = 0;
				for (int k = 0; k < bulletCount[collCheck]; ++k)
				{
					if (Collision(object[i][j], bullet[collCheck][k])) {
						object[i][j]->minusLife(bullet[collCheck][k]);
						bullet[collCheck][k]->SetDeath(true);
					}
				}
			}
		}

		for (int j = 0; j < BUILDING_MAX_COUNT; ++j) {
			if (building[i][j]->GetDeath() == false)					// 건물 - 총알 충돌
			{
				for (int k = 0; k < bulletCount[i]; ++k)
				{
					int collCheck;
					if (i == 0)
						collCheck = 1;
					else
						collCheck = 0;
					for (int l = 0; l < bulletCount[collCheck]; ++l)
					{
						if (Collision(building[i][j], bullet[collCheck][l])) {
							building[i][j]->minusLife(bullet[collCheck][l]);
							bullet[collCheck][l]->SetDeath(true);
						}
					}
				}
			}
		}

		for (int j = 0; j < BUILDING_MAX_COUNT; ++j) {
			if (building[i][j]->GetDeath() == false)					// 건물 - 캐릭터 충돌
			{
				for (int k = 0; k < objectCount[i]; ++k)
				{
					int collCheck;
					if (i == 0)
						collCheck = 1;
					else
						collCheck = 0;
					for (int l = 0; l < objectCount[collCheck]; ++l)
					{
						if (Collision(building[i][j], object[collCheck][l])) {
							building[i][j]->minusLife(object[collCheck][l]);
							object[collCheck][l]->SetDeath(true);
						}

						for (int h = 0; h < arrowCount[collCheck][l]; ++h) {
							if (Collision(building[i][j], arrow[collCheck][l][h])) {
								building[i][j]->minusLife(arrow[collCheck][l][h]);
								arrow[collCheck][l][h]->SetDeath(true);
							}
						}
					}
				}
			}
		}


	}


	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < bulletCount[i]; ++j)
			wallCheck(bullet[i][j]);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//			오브젝트 업데이트
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < objectCount[i]; ++j)
		{
			if (!object[i][j]->GetDeath())
			{
				object[i][j]->update(elapsedTimeInSecond);
				if (arrowTime[i][j] > 0)
					arrowTime[i][j] -= elapsedTimeInSecond;
				else
					AddArrow(i, j);

				for (int h = 0; h < arrowCount[i][j]; ++h) {
					if (!arrow[i][j][h]->GetDeath())
						arrow[i][j][h]->update(elapsedTimeInSecond);
					if (arrow[i][j][h]->GetDeath())
						arrow[i][j][h]->SetPosition(1500, 1500);
				}
			}

			if (object[i][j]->GetDeath()) {
				object[i][j]->SetPosition(1000, 1000);
				for (int h = 0; h < arrowCount[i][j]; ++h) {
					if (!arrow[i][j][h]->GetDeath())
						arrow[i][j][h]->update(elapsedTimeInSecond);
					if (arrow[i][j][h]->GetDeath())
						arrow[i][j][h]->SetPosition(1500, 1500);
				}
			}
		}

		for (int j = 0; j < bulletCount[i]; ++j) {
			if (!bullet[i][j]->GetDeath())
			{
				bullet[i][j]->update(elapsedTimeInSecond);
			}
			if (bullet[i][j]->GetDeath())
				bullet[i][j]->SetPosition(1500, 1500);
		}

		for (int j = 0; j < BUILDING_MAX_COUNT; ++j) {
			if (!building[i][j]->GetDeath())
			{
				building[i][j]->update(elapsedTimeInSecond);
			}
			if (building[i][j]->GetDeath())
				building[i][j]->SetPosition(1500, 1500);
		}
	}
	
	if (bulletTime > 0)
		bulletTime -= elapsedTimeInSecond;
	else
		AddBullet();

	if (objectTime > 0)
		objectTime -= elapsedTimeInSecond;
	else
		AddObject(rand()/250, rand()/400,2,0);
	
	objectCoolTime += elapsedTimeInSecond;

	cout << building[0][1]->GetLife() << endl;

}
