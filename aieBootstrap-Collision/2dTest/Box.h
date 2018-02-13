#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>

using namespace aie;

class Box : public Rigidbody
{
public:
	Box(vec2 position, float length, float height, vec2 velocity, float mass, vec4 color);
	~Box();

	virtual void makeGizmo();


	float getHeight();
	float getLength();

protected:
	vec2 m_corners;
	vec4 m_colors;
	float m_length;
	float m_height;
};

