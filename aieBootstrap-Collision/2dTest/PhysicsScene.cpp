#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

float overlap = 0;

static fn collisionFunctionArray[] =
{
PhysicsScene::plane2Plane, 
PhysicsScene::plane2Sphere,
PhysicsScene::plane2AABB,
PhysicsScene::plane2Box,
PhysicsScene::sphere2Plane,
PhysicsScene::sphere2Sphere,
PhysicsScene::sphere2AABB,
PhysicsScene::sphere2Box,
PhysicsScene::AABB2Plane,
PhysicsScene::AABB2Sphere,
PhysicsScene::AABB2AABB,
PhysicsScene::AABB2Box,
PhysicsScene::box2Plane,
PhysicsScene::box2Sphere,
PhysicsScene::box2AABB,
PhysicsScene::box2Box,
};

PhysicsScene::PhysicsScene() :m_timeStep(0.01f), m_gravity(vec2(0, 0)) {}

PhysicsScene::~PhysicsScene() 
{
	for (auto pActor: m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	for (auto iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
	if (*iter == actor)
	{
	m_actors.erase(iter);
	break;
	}
	}
}

void PhysicsScene::update(float dt)
{
	static list<PhysicsObject*> dirty;

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		/*for (auto pActor : m_actors)
		{
			for (auto pOthers : m_actors)
			{
				if (pActor == pOthers)
					continue;
				if (find(dirty.begin(), dirty.end(), pActor) != dirty.end() && find(dirty.begin(), dirty.end(), pOthers) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid ->checkCollision(pOthers) == true)
				{
					pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOthers), pRigid->getVelocity()*pRigid->getMass());
					dirty.push_back(pRigid);
					dirty.push_back(pOthers);
				}
			}
		}*/
		dirty.clear();
	}
	checkCollision();
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::checkCollision()
{
	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			//using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				//did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject*, PhysicsObject*) { return false; }

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		vec2 displacement = sphere1->getPosition() - sphere2->getPosition();

		vec2 collNormal = normalize(displacement);
		float distance = length(displacement);

		float combinedRadius = sphere1->getRadius() + sphere2->getRadius();
				
		if (distance < combinedRadius)
		{
			float overlap = combinedRadius - distance;

			float totalMass = sphere1->getMass() + sphere2->getMass();

			vec2 sphere1Pos = sphere1->getPosition() + collNormal * ( sphere2->getMass() / totalMass ) * overlap;
			vec2 sphere2Pos = sphere2->getPosition() - collNormal * ( sphere1->getMass() / totalMass ) * overlap;

			sphere1->setPos(sphere1Pos);
			sphere2->setPos(sphere2Pos);
						
			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
			//cout << "Collided" << endl;
			
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere * sphere = dynamic_cast<Sphere*> (obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
		sphere->getRadius();
		
		float intersection = sphere->getRadius() - sphereToPlane;
		
		vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());

		if (intersection > 0)
		{
			cout << "collided" << endl;
			vec2 force = plane->resolveCollision(sphere);

			vec2 position = sphere->getPosition();
			position += (intersection) * (1/sphere->getMass()) * (collisionNormal);

			sphere->setPos(position);
			sphere->applyForce(force, contact);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*> (obj1);
	AABB *aabb = dynamic_cast<AABB*> (obj2);
	
	if (sphere != nullptr && aabb != nullptr)
	{
		vec2 normal = sphere->getPosition() - aabb->getPosition();

		vec2 closest = normal;

		vec2 max_extent = aabb->getPosition() + vec2(aabb->getLength(), aabb->getHeight());
		vec2 min_extent = aabb->getPosition() - vec2(aabb->getLength(), aabb->getHeight());
				
		closest = clamp(sphere->getPosition(), min_extent, max_extent);

		Gizmos::add2DCircle(closest, 1, 12, vec4(1, 1, 1, 1));
		//float distance = closest.length() * closest.length();
		vec2 distance = closest - sphere->getPosition();

		float Length = length(distance);

		float radius = sphere->getRadius();

		
		if (Length < radius)
		{
 			cout << "Collided with AABB to Sphere" << endl;
			
			/*float overlap = (length(distance) + radius) - distanceMagnitude;

			float totalMass = sphere->getMass() + aabb->getMass();

			vec2 spherePos = sphere->getPosition() - normalize(normal) * (sphere->getMass() / totalMass) * overlap;
			vec2 aabbPos = aabb->getPosition() + normalize(normal) * (aabb->getMass() / totalMass) * overlap;

			sphere->setPos(spherePos);
			aabb->setPos(aabbPos);*/

			aabb->resolveCollision(sphere, closest);
			sphere->resolveCollision(aabb, closest);

			//aabb->resolveCollision(sphere, normal);
			
			return true;
		}

	}
	return false;
}

bool PhysicsScene::AABB2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2AABB(obj2, obj1);
}

bool PhysicsScene::plane2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane *plane = dynamic_cast<Plane*> (obj1);
	AABB *aabb = dynamic_cast<AABB*> (obj2);

	if (plane != nullptr && aabb != nullptr)
	{
		vec2 normal = plane->getNormal();
		//vec2 aabbBL = aabb->getPosition();
		//vec2 aabbBR = aabb->getPosition() + vec2(aabb->getLength(), 0);
		//vec2 aabbTL = aabb->getPosition() + vec2(0, aabb->getHeight());
		//vec2 aabbTR = aabb->getPosition() + vec2(aabb->getLength(), aabb->getHeight());
		vec2 aabbBL = aabb->getPosition() - vec2(aabb->getLength(), aabb->getHeight());
		vec2 aabbBR = aabb->getPosition() + vec2(aabb->getLength(), -1.0f*(aabb->getHeight()));
		vec2 aabbTL = aabb->getPosition() + vec2(-1.0f*(aabb->getLength()), aabb->getHeight());
		vec2 aabbTR = aabb->getPosition() + vec2(aabb->getLength(), aabb->getHeight());

		if (dot(normal, aabbBL) - plane->getDistance() < 0 ||
			dot(normal, aabbBR) - plane->getDistance() < 0 ||
			dot(normal, aabbTL) - plane->getDistance() < 0 ||
			dot(normal, aabbTR) - plane->getDistance() < 0)
		{
			if (dot(normal, aabbBL) - plane->getDistance() < 0)
				overlap = dot(normal, aabbBL) - plane->getDistance();

			else if (dot(normal, aabbBR) - plane->getDistance() < 0)
				overlap = dot(normal, aabbBR) - plane->getDistance();

			else if (dot(normal, aabbTL) - plane->getDistance() < 0)
				overlap = dot(normal, aabbTL) - plane->getDistance();

			else if (dot(normal, aabbTR) - plane->getDistance() < 0)
				overlap = dot(normal, aabbTR) - plane->getDistance();

			vec2 force = plane->resolveCollision(aabb);

			vec2 position = aabb->getPosition();
			position -= (overlap) * (normal);

			aabb->setPos(position);
			aabb->applyForce(force, normal);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::AABB2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return plane2AABB(obj2, obj1);
}


bool PhysicsScene::AABB2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	AABB *aabb1 = dynamic_cast<AABB*> (obj2);
	AABB *aabb2 = dynamic_cast<AABB*> (obj1);

	vec2 normal = aabb2->getPosition() - aabb1->getPosition();

	if (aabb1 != nullptr  && aabb2 != nullptr)
	{
		vec2 aabb1ColliderMax = aabb1->getPosition() + vec2(aabb1->getLength(), aabb1->getHeight());
		vec2 aabb1ColliderMin = aabb1->getPosition() - vec2(aabb1->getLength(), aabb1->getHeight());

		vec2 aabb2ColliderMax = aabb2->getPosition() + vec2(aabb2->getLength(),aabb2->getHeight());
		vec2 aabb2ColliderMin = aabb2->getPosition() - vec2(aabb2->getLength(), aabb2->getHeight());

		if (!(aabb1ColliderMax.x < aabb2ColliderMin.x ||
			  aabb2ColliderMax.x < aabb1ColliderMin.x ||
			  aabb1ColliderMax.y < aabb2ColliderMin.y ||
			  aabb2ColliderMax.y < aabb1ColliderMin.y))
		{
			float AABBOverlap = 0;

			if (!(aabb1ColliderMax.x < aabb2ColliderMin.x))
				AABBOverlap = aabb1ColliderMax.x - aabb2ColliderMin.x;

			else if (!(aabb2ColliderMax.x < aabb1ColliderMin.x))
				AABBOverlap = aabb2ColliderMax.x - aabb1ColliderMin.x;

			else if (!(aabb1ColliderMax.y < aabb2ColliderMin.y))
				AABBOverlap = aabb1ColliderMax.y - aabb2ColliderMin.y;

			else if (!(aabb2ColliderMax.y < aabb1ColliderMin.y))
				AABBOverlap = aabb2ColliderMax.y - aabb1ColliderMin.y;

			vec2 contact = normalize(normal) * AABBOverlap;

			Gizmos::add2DCircle(contact, 1, 12, vec4(0, 1, 0, 1));

			aabb1->resolveCollision(aabb2,contact);
			cout << "Collided aabb" << endl;
			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box *box = dynamic_cast<Box*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * fminf(box->getLength(), box->getHeight());

		vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = dot(box->getPosition() - planeOrigin, plane->getNormal());

		for (float x = -box->getExtents().x; x < box->getLength(); x += box->getLength())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
			{
				vec2 p = box->getPosition() + box->getLocalX() + y * box->getLocalY();

				float distFromPlane = dot(p - planeOrigin, plane->getNormal());

				float velocityIntoPlane = dot(box->getVelocity() + box->getRotation() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) ||
					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		if (numContacts > 0)
		{
			float collisionV = contactV / (float)numContacts;
			vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);
			vec2 localContact = (contact / (float)numContacts) - box->getPosition();

			float r = dot(localContact, vec2(plane->getNormal().y, -plane->getNormal().x));

			float mass0 = 1.0f / (1.0f / box->getMass() + (r*r) / box->getMoment());

			box->applyForce(acceleration * mass0, localContact);
		}
	}

	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return box2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2) 
{
	return true;
}

bool PhysicsScene::AABB2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return true;
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);

	if (box != nullptr && sphere != nullptr)
	{
		vec2 circlePos = sphere->getPosition() - box->getPosition();
		float w2 = box->getLength() / 2;
		float h2 = box->getHeight() / 2;

		int numContacts = 0;
		vec2 contact(0, 0);

		for (float x = -w2; x <= w2; x += box->getLength())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				vec2 p = x * box->getLocalX() + y * box->getLocalY();
				vec2 dp = p - circlePos;
				if (dp.x * dp.x + dp.y * dp.y < sphere->getRadius() * sphere->getRadius())
				{
					numContacts++;
					contact += vec2(x, y);
				}
			}
		}
		vec2* direction = nullptr;
		vec2 localPos(dot(box->getLocalX(), circlePos), dot(box->getLocalY(), circlePos));

		if (localPos.y <h2 && localPos.y > -h2)
		{
			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius())
			{
				numContacts++;
				contact += vec2(w2, localPos.y);
				direction = new vec2(box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius())) 
			{
				numContacts++;
				contact += glm::vec2(-w2, localPos.y); 
				direction = new glm::vec2(-box->getLocalX()); 
			}
		}
		if (localPos.x < w2 && localPos.x > -w2)
		{
			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->getLocalY());
			}
		}
		if (numContacts > 0) 
		{
			contact = box->getPosition() + (1.0f / numContacts) * (box->getLocalX() *contact.x + box->getLocalY()*contact.y); 
			box->resolveCollision(sphere, contact, direction);
		}
		delete direction;
	}
	return false;
}

bool PhysicsScene::box2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return true;
}

bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject*obj2) 
{
	return true;
}