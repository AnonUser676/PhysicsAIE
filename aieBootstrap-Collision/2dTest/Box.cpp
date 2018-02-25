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

bool Box::checkBoxCorners(Box& box, vec2& contact, int& numContacts, float &pen, vec2& edgeNormal)
{
	float minX, maxX, minY, maxY; 
	float boxW = box.getExtents().x * 2; 
	float boxH = box.getExtents().y * 2; 
	int numLocalContacts = 0; 
	vec2 localContact(0, 0);
	bool first = true; 
	
	for (float x = -box.getExtents().x; x < boxW; x += boxW) 
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH) 
		{ 
			// position in worldspace
			vec2 p = box.getPosition() + x*box.m_localX + y*box.m_localY; 
			
			// position in our box's space
			vec2 p0( dot(p - m_position, m_localX), dot(p - m_position, m_localY) );
			
			if (first || p0.x < minX)
				minX = p0.x; 
			
			if (first || p0.x > maxX) 
				maxX = p0.x; 
			
			if (first || p0.y < minY) 
				minY = p0.y; 
			
			if (first || p0.y > maxY) 
				maxY = p0.y; 
			
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{ 
				numLocalContacts++; 
				localContact += p0; 
			} 
			
			first = false; 
		} 
	} 
	
	if (maxX <-m_extents.x || minX >m_extents.x || maxY<-m_extents.y || minY >m_extents.y) 
		return false; 
	
	if (numLocalContacts == 0) 
		return false; 
	
	bool res = false; 
	contact += m_position + (localContact.x*m_localX + localContact.y*m_localY) / (float)numLocalContacts; 
	numContacts++; 
	float pen0 = m_extents.x - minX; 
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = m_localX; pen = pen0; 
		res = true; 
	} 
	
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = -m_localX; 
		pen = pen0; 
		res = true; 
	} 
	
	pen0 = m_extents.y - minY; 
	
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = m_localY; 
		pen = pen0; 
		res = true; 
	} 
	
	pen0 = maxY + m_extents.y; 
	
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = -m_localY; 
		pen = pen0; 
		res = true; 
	} 
	
	return res;
}