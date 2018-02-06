#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(vec2 force);
	void applyForceToActor(Rigidbody* actor, vec2 force);

	void setMass(float mass) { m_mass = mass; }

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }

protected:
	vec2 m_position;
	vec2 m_velocity;
	float m_mass;
	float m_rotation;
};

