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
	Plane* wall;
	Sphere* UFO;
	Box* square;

protected:

	Renderer2D*	m_2dRenderer;
	Font*			m_font;
	PhysicsScene*	m_physicsScene;
	setupContinuousDemo*  m_ContinousDemo;
	
	float Timer;
};