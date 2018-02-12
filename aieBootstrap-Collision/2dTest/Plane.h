#pragma once
#include "PhysicsObject.h"
#include <Gizmos.h>

using namespace glm;
using namespace aie;

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(vec2 normal, float distance);
	~Plane();

	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void debug() {};
	virtual void makeGizmo();
	virtual void resetPosition();

	vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distancetoOrigin; }
	
protected:
	vec2 m_normal;
	float m_distancetoOrigin;
};

