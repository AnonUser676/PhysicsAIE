#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>

using namespace aie;

class AABB : public Rigidbody
{
public:
	AABB(vec2 position, float length, float height, vec2 velocity, float mass, float linearDrag, float angularDrag, float elasticity, vec4 color);
	AABB(vec2 position, float inclination, float speed, float mass, float length, float height, float linearDrag, float angularDrag, float elasticity, vec4 color);
	~AABB();

	virtual void makeGizmo();

	float getHeight();
	float getLength();

protected:
	vec2 m_corners;
	vec4 m_colors;
	float m_length;
	float m_height;
};

