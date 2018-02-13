#include "Box.h"

Box::Box(vec2 position, float length, float height, vec2 velocity, float mass, vec4 color):Rigidbody(BOX,position,velocity,0,mass)
{
	m_position = position;
	m_velocity = velocity;
	m_length = length;
	m_height = height;
	m_mass = mass;
	m_colors = color;
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