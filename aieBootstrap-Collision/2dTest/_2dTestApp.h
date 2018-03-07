#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

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

	Plane* wall;
	Plane* wall2;
	Plane* wall3;
	Plane* wall4;
	Plane* wall5;
	Box* objectBox1;
	Box* objectBox2;

	vector<Sphere*> circle;
	vector<AABB*> aabb;
	Box* boX;

protected:

	Renderer2D*	m_2dRenderer;
	Font*			m_font;
	PhysicsScene*	m_physicsScene;
	float Timer;
	float gravity;
	float SpherePosX;
	float SpherePosY;
	int SphereVelX;
	int SphereVelY;
	float SphereMass;
	float Radius;
	float SphereLinearDrag;
	float SphereElasticity;
	bool  sphereStatic;
	vec4 SphereColor;
	
	float AABBPosX;
	float AABBPosY;
	int AABBVelX;
	int AABBVelY;
	float AABBMass;
	float AABBLength;
	float AABBHeight;
	float AABBLinearDrag;
	float AABBElasticity;
	bool  AABBStatic;
	vec4 AABBColor;

	int circleCounter;
	int aabbCounter;

};