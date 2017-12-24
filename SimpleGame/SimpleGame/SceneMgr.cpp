#include "stdafx.h"
#include "SceneMgr.h"


default_random_engine dre;
uniform_int_distribution<int> ranPos(-250, 250);

Renderer *renderer = NULL;

SceneMgr::SceneMgr()
{
	renderer = new Renderer(500, 800);

	m_sound = new Sound();


	objectCount[0] = 0;
	objectCount[1] = 0;
	bulletCount[0] = 0;
	bulletCount[1] = 0;

	animateCount[0] = 0;
	animateCount[1] = 0;

	texCharacter[0] = renderer->CreatePngTexture("../Resource/ohhh.png");
	texCharacter[1] = renderer->CreatePngTexture("../Resource/ohh.png");
	texCharacter[2] = renderer->CreatePngTexture("../Resource/1.png");	// 적군
	texCharacter[3] = renderer->CreatePngTexture("../Resource/2.png");	// 아군
	texCharacter[4] = renderer->CreatePngTexture("../Resource/3.png");	// 총알 파티클
	texCharacter[5] = renderer->CreatePngTexture("../Resource/Snow.png");	// 눈
	texGrass = renderer->CreatePngTexture("../Resource/Grass.png");
	
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 10; ++j) {
			arrowCount[i][j] = 0;
			arrowTime[i][j] = 0;
		}
	}

	objectCoolTime = 7;
	particleTime = 0;
}

SceneMgr::~SceneMgr()
{
}
 
void SceneMgr::Release() 
{
	dre.seed(time(NULL));


	if (!renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < BUILDING_MAX_COUNT; ++j)
			building[i][j] = new Object(-150 + 150 * j, -600 * i + 300, OBJECT_BUILDING);
	}

	soundBG = m_sound->CreateSound("./Dependencies/SoundSamples/MF-W-90.XM");

	m_sound->PlaySound(soundBG, true, 0.2f);

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
		sceneTransform = 1;
		return true;
	}

	return 0;
}

void SceneMgr::Darw(float elapsedTimeInSecond)
{
	renderer->DrawTexturedRect(0, 0, 0, 800, 1, 1, 1, 1, texGrass, 0.4);	// 배경 Depth 최대값. 1이상은 출력 안됨.

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
			if(i==0)
				renderer->DrawTexturedRectSeq(object[i][j]->GetX(), object[i][j]->GetY(), 0, 50, 1, 1, 1, 1, texCharacter[2], animateCount[0]/3, 0, 8, 1, 0.1);
			else
				renderer->DrawTexturedRectSeq(object[i][j]->GetX(), object[i][j]->GetY(), 0, 50, 1, 1, 1, 1, texCharacter[3], animateCount[0] / 3, animateCount[1], 8, 8, 0.1);
			renderer->DrawSolidRectGauge(object[i][j]->GetX(), object[i][j]->GetY() + 30, 0, 20, 3, bullet[i][j]->GetR(), bullet[i][j]->GetG(), bullet[i][j]->GetB(), 1, (float)object[i][j]->GetLife() / 100, 0.2);
			object[i][j]->SetCollision(false);

			for (int h = 0; h < arrowCount[i][j]; ++h) {
				if(i==0)
					renderer->DrawSolidRect(arrow[i][j][h]->GetX(), arrow[i][j][h]->GetY(), 0, arrow[i][j][h]->GetSize(), arrow[i][j][h]->GetR(), arrow[i][j][h]->GetG(), arrow[i][j][h]->GetB(), 1, 0.3);
				else
					renderer->DrawSolidRect(arrow[i][j][h]->GetX(), arrow[i][j][h]->GetY(), 0, arrow[i][j][h]->GetSize(), arrow[i][j][h]->GetR(), arrow[i][j][h]->GetG(), arrow[i][j][h]->GetB(), 1, 0.3);
			}
		}

		for (int j = 0; j < bulletCount[i]; ++j) {
			renderer->DrawParticle(bullet[i][j]->GetX(), bullet[i][j]->GetY(), 0, 5, 1, 1, 1, 1, -(bullet[i][j]->GetMoveX()), -(bullet[i][j]->GetMoveY()), texCharacter[4], particleTime,0.3);
			renderer->DrawSolidRect(bullet[i][j]->GetX(), bullet[i][j]->GetY(), 0, 5, bullet[i][j]->GetR(), bullet[i][j]->GetG(), bullet[i][j]->GetB(), 1, 0.3);
		}
	}

	renderer->DrawText(0, 0, GLUT_BITMAP_9_BY_15, 1, 0, 0, "Ohh123");

	renderer->DrawParticleClimate(0, 0, 0, 1, 1, 1, 1, 1, -0.1, -0.1, texCharacter[5], particleTime, 0.01);

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
							renderer->SetSceneTransform(10, 0, 1, 1);
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

	/////////////////////////////////////////////////////////
	// 애니메이션
	if (animateCount[0] < 24)
		++animateCount[0];
	else
	{
		animateCount[0] = 0;
		if (animateCount[1] < 8)
			++animateCount[1];
		else
			animateCount[1] = 0;
	}
	particleTime += 0.1 * ((float) elapsedTimeInSecond);
	SceneTransform();
}

void SceneMgr::SceneTransform() {
	if (sceneTransform == 1)
	{
		renderer->SetSceneTransform(30, 0, 1, 1);
		++sceneTransform;
	}
	else if (sceneTransform == 20) {
		renderer->SetSceneTransform(-30, 0, 1, 1);
		sceneTransform = 0;
	}
	else if (sceneTransform > 0) {
		++sceneTransform;
	}
}