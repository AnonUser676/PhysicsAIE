#include "Sphere.h"


Sphere::Sphere(vec2 position, vec2 velocity, float mass, float radius, vec4 color):Rigidbody(SPHERE, position,velocity,0,mass)
{
	m_radius = radius;
	m_color = color;
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
}

Sphere::Sphere(vec2 position, float inclination, float speed, float mass, float radius, vec4 color) :Sphere(position, vec2(cosf(inclination), sinf(inclination))*speed, mass, radius, color)
{
	
}


Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	Gizmos::add2DCircle(m_position, m_radius, 36, m_color);
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