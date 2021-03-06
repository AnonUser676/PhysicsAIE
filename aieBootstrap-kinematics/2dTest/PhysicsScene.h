#pragma once
#include <vector>
#include <glm/ext.hpp>
#include "PhysicsObject.h"
#include <list>
#include "Rigidbody.h"
#include <iostream>
#include "Sphere.h"

using namespace std;
using namespace glm;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor) 
	{
		m_actors.push_back(actor);
	};

	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();
	void debugScene();

	void setGravity(const vec2 gravity) { m_gravity = gravity; }
	vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

protected:
	vec2 m_gravity;
	float m_timeStep;
	vector<PhysicsObject*> m_actors;
	
};

