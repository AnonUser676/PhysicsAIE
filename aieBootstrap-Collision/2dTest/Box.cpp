#include "Box.h"

Box::Box(vec2 position, float length, float height, vec2 velocity, float mass, float linearDrag, float angularDrag, float elasticity, vec4 color):Rigidbody(BOX,position,velocity,0,mass, linearDrag, angularDrag, elasticity)
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
	m_extents = vec2((length) * 0.5f, (height) * 0.5f);
	//m_localX = normalize(vec2(cosf(m_rotation), sinf(m_rotation)));
	//m_localY = normalize(vec2(-sinf(m_rotation), cosf(m_rotation)));
}

Box::Box(vec2 position, float length, float height, float inclination, float speed,  float mass, float linearDrag, float angularDrag, float elasticity, vec4 color) :Box(position,length,height, vec2(cosf(inclination), sinf(inclination))*speed, mass, linearDrag,angularDrag,elasticity,color)
{
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	//mat4 transform = rotate(m_rotation, vec3(0, 0, 1));
	
	vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

	Gizmos::add2DTri(p1, p2, p4, m_colors);
	Gizmos::add2DTri(p1, p4, p3, m_colors);
}

void Box::fixedUpdate(vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = normalize(vec2(cs, sn));
	m_localY = normalize(vec2(-sn, cs));
}

float Box::getHeight()
{
	return m_height;
}

float Box::getLength()
{
	return m_length;
}

vec2 Box::getLocalX()
{
	return m_localX;
}

vec2 Box::getLocalY()
{
	return m_localY;
}


vec2 Box::getExtents()
{
	return vec2(m_length,m_height);
}