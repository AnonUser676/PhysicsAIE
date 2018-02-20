#include "Rigidbody.h"

#define MIN_LINEAR_THRESHOLD 0.000000001f
#define MIN_ROTATION_THRESHOLD 0.01f

Rigidbody::Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity):PhysicsObject(shapeID)
{
	m_rotation = rotation;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	m_angularVelocity = 0.01f;
	m_elasticity = elasticity;
	m_moment = 0;
}

Rigidbody::~Rigidbody(){}

void Rigidbody::fixedUpdate(vec2 gravity, float timeStep)
{
	//if (m_isKinematic)
	//	return;
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag *timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = vec2(0, 0);
	}

	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity = 0;
	}

	//applyForce(gravity * m_mass * timeStep);
	//m_position += m_velocity * timeStep;
}

void Rigidbody::resolveCollision(Rigidbody* actor2, vec2 contact, vec2* collisionNormal)
{
	vec2 normal = collisionNormal ? *collisionNormal : normalize(actor2->m_position - m_position);

	vec2 perp(normal.y, -normal.x);

	float r1 = dot(contact - m_position, -perp);
	float r2 = dot(contact - actor2->m_position, -perp);

	float v1 = dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	v1 = abs(v1);
	v2 = abs(v2);

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2*r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		vec2 force = (1.0f + elasticity)*mass1*mass2 / (mass1 + mass2)*(v1 - v2)*normal;

		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}

	//vec2 normal = normalize(actor2->getPosition() - m_position);
	//vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	//
	//float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
	//float j = dot(-(1 + elasticity) * (relativeVelocity), normal) / dot(normal, normal*((1 / m_mass) + 1 / actor2->getMass()));
	//
	//vec2 force = normal * j;
	//
	//applyForceToActor(actor2, force);
}

void Rigidbody::debug(){}

void Rigidbody::applyForce(vec2 force, vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y*pos.x - force.x * pos.y) / (m_moment);
}

void Rigidbody::applyForceToActor(Rigidbody* actor, vec2 force)
{
	//actor->applyForce(force);
	//this->applyForce(-force);
}