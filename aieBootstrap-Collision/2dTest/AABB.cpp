#include "AABB.h"



AABB::AABB(vec2 position, float length, float height, vec2 velocity, float mass, float linearDrag, float angularDrag, float elasticity, vec4 color) :Rigidbody(AABBSquare, position, velocity, 0, mass, linearDrag, angularDrag, elasticity)
{
	m_position = position;
	m_velocity = velocity;
	m_length = length;
	m_height = height;
	m_mass = mass;
	m_colors = color;
	m_moment = 1.0f / 12.0f * mass * length * height;
	m_angularDrag = angularDrag;
	m_linearDrag = linearDrag;
	m_elasticity = elasticity;
}

AABB::AABB(vec2 position, float inclination, float speed, float mass, float length, float height, float linearDrag, float angularDrag, float elasticity, vec4 color) :AABB(position, length, height, vec2(cosf(inclination), sinf(inclination))*speed, mass, linearDrag, angularDrag, elasticity, color)
{
}

AABB::~AABB()
{
}

void AABB::makeGizmo()
{
	Gizmos::add2DAABBFilled(m_position, vec2(m_length, m_height), m_colors);
}

float AABB::getHeight()
{
	return m_height;
}

float AABB::getLength()
{
	return m_length;
}