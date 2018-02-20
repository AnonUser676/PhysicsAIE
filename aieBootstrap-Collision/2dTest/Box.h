#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>

using namespace aie;

class Box : public Rigidbody
{
public:
	Box(vec2 position, float length, float height, vec2 velocity, float mass, float linearDrag, float angularDrag, float elasticity, vec4 color);
	Box(vec2 position, float inclination, float speed, float length, float height, float mass, float linearDrag, float angularDrag, float elasticity, vec4 color);
	~Box();

	virtual void makeGizmo();

	virtual void fixedUpdate(vec2 gravity, float timeStep);

	float getHeight();
	float getLength();
	vec2 getLocalX();
	vec2 getLocalY();
	vec2 getExtents();

protected:
	vec2 m_corners;
	vec4 m_colors;
	float m_length;
	float m_height;
	vec2 m_extents;
	vec2 m_localX;
	vec2 m_localY;
};

