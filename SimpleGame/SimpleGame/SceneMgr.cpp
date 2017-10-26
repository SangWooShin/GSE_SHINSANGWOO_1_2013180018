#include "stdafx.h"
#include "SceneMgr.h"

Renderer *g_Renderer = NULL;

SceneMgr::SceneMgr()
{
	objectCount = 0;
}

SceneMgr::~SceneMgr()
{
}

void SceneMgr::Release() {
	g_Renderer = new Renderer(500, 500);
	if (!g_Renderer->IsInitialized())
	{
	std::cout << "Renderer could not be initialized.. \n";
	}
}

void SceneMgr::Delete() {
	delete g_Renderer;
}

void SceneMgr::AddObject(float x, float y, float moveX, float moveY) {
	if (objectCount < 50) {
		m_object[objectCount] = new Object(x,y, moveX, moveY);
		++objectCount;
	}
}

void SceneMgr::Collision(int inputNum) {
	if (m_object[inputNum]->GetX() < -250 || m_object[inputNum]->GetX() > 250)
		m_object[inputNum]->SetMoveX((-1)*m_object[inputNum]->GetMoveX());
	if (m_object[inputNum]->GetY() < -250 || m_object[inputNum]->GetY() > 250)
		m_object[inputNum]->SetMoveY((-1)*m_object[inputNum]->GetMoveY());

	for (int i = 0; i < 50; ++i) {
		if (inputNum != i && m_object[inputNum]->GetX() - m_object[i]->GetX() < 5 && m_object[inputNum]->GetX() - m_object[i]->GetX() > -5
			&& m_object[inputNum]->GetY() - m_object[i]->GetY() < 5 && m_object[inputNum]->GetY() - m_object[i]->GetY() > -5)
			m_object[inputNum]->SetCollision(true);
	}
}

void SceneMgr::Darw() {
	for (int i = 0; i < 50; ++i) {
		if (m_object[i]->GetDeath() == false) {
			Collision(i);
			if (m_object[i]->GetCollision() == true)
				g_Renderer->DrawSolidRect(m_object[i]->GetX(), m_object[i]->GetY(), 0, 10, 1, 0, 0, 1);	// (x,y,z,크기,r,g,b,a)
			else
				g_Renderer->DrawSolidRect(m_object[i]->GetX(), m_object[i]->GetY(), 0, 10, 1, 1, 1, 1);	// (x,y,z,크기,r,g,b,a)
			m_object[i]->SetCollision(false);
		}
	}
}

void SceneMgr::Update(DWORD elapsedTime) {
	for (int i = 0; i < 50; ++i) {
		m_object[i]->Move();
		m_object[i]->update(elapsedTime);
	}
}
