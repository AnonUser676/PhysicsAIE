#include "Box.h"

Box::Box(vec2 position, float length, float height, vec2 velocity, float mass, float linearDrag, float angularDrag, float elasticity, vec4 color):Rigidbody(BOX,position,velocity,0,mass, linearDrag, angularDrag, elasticity)
{
	m_position = position;
	m_velocity = velocity;
	m_length = length;
	m_height = height;
	m_mass = mass;
	m_colors = color;
	m_angularDrag = angularDrag;
	m_linearDrag = linearDrag;
	m_elasticity = elasticity;
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	Gizmos::add2DAABBFilled(m_position, vec2(m_length, m_height), m_colors);
}

float Box::getHeight()
{
	return m_height;
}

float Box::getLength()
{
	return m_length;
}