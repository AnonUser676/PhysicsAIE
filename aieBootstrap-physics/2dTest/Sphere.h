#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>

using namespace aie;

class Sphere : public Rigidbody
{
public:
	Sphere(vec2 position, vec2 velocity, float mass, float radius, vec4 color);
	~Sphere();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }
	vec4 getColor() { return m_color; }

protected:
	float m_radius;
	vec4 m_color;
};

