#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "setupContinuousDemo.h"

using namespace aie;
using namespace std;
using namespace glm;

class _2dTestApp : public aie::Application {
public:

	_2dTestApp();
	virtual ~_2dTestApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	virtual void imgui();

	Sphere* rocket;
	Sphere* ball1;
	Sphere* ball2;
	Plane* wall;
	Sphere* UFO;
	Box* square;
	Box* box;
	Plane* wall2;
	Plane* wall3;
	Plane* wall4;
	Plane* wall5;

protected:

	Renderer2D*	m_2dRenderer;
	Font*			m_font;
	PhysicsScene*	m_physicsScene;
	setupContinuousDemo*  m_ContinousDemo;
	
	float Timer;
};