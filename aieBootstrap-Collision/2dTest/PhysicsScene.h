#pragma once
#include <vector>
#include <glm/ext.hpp>
#include "PhysicsObject.h"
#include <list>
#include "Rigidbody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Box.h"

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

	void setGravity(vec2 gravity) { m_gravity = gravity; }
	vec2 getGravity() { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkCollision();

	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool plane2AABB(PhysicsObject*, PhysicsObject*);
	static bool sphere2AABB(PhysicsObject*, PhysicsObject*);
	static bool AABB2Plane(PhysicsObject*, PhysicsObject*);
	static bool AABB2Sphere(PhysicsObject*, PhysicsObject*);
	static bool AABB2AABB(PhysicsObject*, PhysicsObject*);
	static bool box2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Box(PhysicsObject*, PhysicsObject*);
	static bool sphere2Box(PhysicsObject*, PhysicsObject*);
	static bool AABB2Box(PhysicsObject*, PhysicsObject*);
	static bool box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool box2AABB(PhysicsObject*, PhysicsObject*);
	static bool box2Box(PhysicsObject*, PhysicsObject*);
	int actorCounter() { return m_actors.size(); }

protected:
	vec2 m_gravity;
	float m_timeStep;
	vector<PhysicsObject*> m_actors;
	
};

