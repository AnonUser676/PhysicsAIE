#include "Sphere.h"


Sphere::Sphere(vec2 position, vec2 velocity, float mass, float radius, float linearDrag, float angularDrag, float elasticity, vec4 color):Rigidbody(SPHERE, position,velocity,0,mass,linearDrag,angularDrag, elasticity)
{
	m_radius = radius;
	m_color = color;
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	m_moment = 0.5f * mass * radius * radius;
	m_elasticity = elasticity;
	setKinematic(false);
}

Sphere::Sphere(vec2 startPos, float inclination, float speed, float mass, float radius, float linearDrag, float angularDrag, float elasticity, vec4 color) :Sphere(startPos, vec2(cosf(inclination), sinf(inclination))*speed, mass, radius, linearDrag, angularDrag, elasticity, color)
{
	
}


Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	vec2 end = vec2(cos(m_rotation), sin(m_rotation))*m_radius;

	Gizmos::add2DCircle(m_position, m_radius, 36, m_color);
	Gizmos::add2DLine(m_position, m_position + end, -m_color);
}

//bool Sphere::checkCollision(PhysicsObject* pOther)
//{
//	Sphere* other = dynamic_cast<Sphere*>(pOther);
//	if (other)
//	{
//		return ((other->m_radius + this->m_radius) > distance(other->m_position, this->m_position));
//	}
//	return false;
//}