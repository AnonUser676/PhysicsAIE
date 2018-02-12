#include "Plane.h"



Plane::Plane(): PhysicsObject(PLANE)
{
	m_distancetoOrigin = 0;
	m_normal = vec2(0, 1);
}

Plane::Plane(vec2 normal, float distance) : Plane()
{
	m_normal = normal;
	m_distancetoOrigin = distance;
}

Plane::~Plane()
{
}

void Plane::fixedUpdate(vec2 gravity, float timeStep)
{
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = m_normal * m_distancetoOrigin;
	vec2 parallel(m_normal.y, -m_normal.x);
	float color1 = 0;
	float color2 = 1;
	float color3 = 0;
	vec4 colour(color1, color2, color3, 1);
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	Gizmos::add2DLine(start, end, colour);
}

void Plane::resetPosition() {}