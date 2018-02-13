#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, float rotation, float mass):PhysicsObject(shapeID)
{
	m_rotation = rotation;
}

Rigidbody::~Rigidbody(){}

void Rigidbody::fixedUpdate(vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;

}

void Rigidbody::resolveCollision(Rigidbody* actor2)
{
	vec2 normal = normalize(actor2->getPosition() - m_position);
	vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = 1;
	float j = dot(-(1 + elasticity) * (relativeVelocity), normal) / dot(normal, normal*((1 / m_mass) + 1 / actor2->getMass()));

	vec2 force = normal * j;

	applyForceToActor(actor2, force);
}

void Rigidbody::debug(){}

void Rigidbody::applyForce(vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody* actor, vec2 force)
{
	actor->applyForce(force);
	this->applyForce(-force);
}