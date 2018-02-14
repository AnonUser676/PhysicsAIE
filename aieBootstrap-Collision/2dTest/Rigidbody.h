#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float elasticity);
	~Rigidbody();

	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(vec2 force);
	void applyForceToActor(Rigidbody* actor, vec2 force);

	void setMass(float mass) { m_mass = mass; }
	void setVelocity(vec2 velocity) { m_velocity = velocity; }
	void resolveCollision(Rigidbody* actor2);
	void setPos(vec2 position) { m_position = position; }
	//virtual bool checkCollision(PhysicsObject* pOther) = 0;

	vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getLinearDrag() { return m_linearDrag; }
	float getAngularDrag() { return m_angularDrag; }
	float getAngularVelocity() { return m_angularVelocity; }
	float getElasticity() { return m_elasticity; }

protected:
	vec2 m_position;
	vec2 m_velocity;
	float m_mass;
	float m_rotation;
	float m_linearDrag;
	float m_angularDrag;
	float m_angularVelocity;
	float m_elasticity;
};

