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