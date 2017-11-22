/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "Object.h"
#include "Renderer.h"
#include "SceneMgr.h"

SceneMgr* sceneMgr = NULL;

DWORD prevTime = 0;
bool lButtonDown;
#define WINDOW_WIDTH_MAX 500
#define WINDOW_HIGHT_MAX 800

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	DWORD currTime = timeGetTime();
	float elapsedTime = (currTime - prevTime) / 1000.f;
	prevTime = currTime;
	
	sceneMgr->Update(elapsedTime);
	sceneMgr->Darw();
	
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		lButtonDown = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && lButtonDown == true)
	{
		if(-y + WINDOW_HIGHT_MAX / 2 < 0)
			sceneMgr->AddObject(x - WINDOW_WIDTH_MAX / 2, -y + WINDOW_HIGHT_MAX / 2, 2, 1);
		lButtonDown = false;
	}

	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH_MAX, WINDOW_HIGHT_MAX);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	sceneMgr = new SceneMgr;

	sceneMgr->Release();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	prevTime = timeGetTime();

	glutMainLoop();

	delete sceneMgr;

    return 0;
}

