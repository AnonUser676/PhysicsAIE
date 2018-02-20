#pragma once
#include <glm/ext.hpp>

using namespace glm;

enum ShapeType
{
	PLANE,
	SPHERE,
	AABBSquare,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID);
	

	ShapeType m_shapeID;

public:
	virtual void fixedUpdate(vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetposition() {};
	ShapeType getShapeID() { return m_shapeID; }
	~PhysicsObject();
};

