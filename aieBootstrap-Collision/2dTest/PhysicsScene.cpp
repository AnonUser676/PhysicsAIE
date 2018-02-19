#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

vec2 clamping(const vec2 vector, const vec2 & min, const vec2 & max);
float overlap = 0;

static fn collisionFunctionArray[] =
{
PhysicsScene::plane2Plane, 
PhysicsScene::plane2Sphere,
PhysicsScene::plane2Box,
PhysicsScene::sphere2Plane,
PhysicsScene::sphere2Sphere,
PhysicsScene::sphere2Box,
PhysicsScene::box2Plane,
PhysicsScene::box2Sphere,
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
						
			sphere1->resolveCollision(sphere2);
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
		

		if (intersection > 0)
		{
			cout << "collided" << endl;
			vec2 force = plane->resolveCollision(sphere);

			vec2 position = sphere->getPosition();
			position += (intersection) * (1/sphere->getMass()) * (collisionNormal);

			sphere->setPos(position);
			sphere->applyForce(force);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*> (obj1);
	Box *box = dynamic_cast<Box*> (obj2);
	
	if (sphere != nullptr && box != nullptr)
	{
		vec2 normal = sphere->getPosition() - box->getPosition();

		vec2 closest = normal;

		vec2 max_extent = box->getPosition() + vec2(box->getLength(), box->getHeight());
		vec2 min_extent = box->getPosition() - vec2(box->getLength(), box->getHeight());

		closest = clamp(max_extent, min_extent, sphere->getPosition());
				
		//float distance = closest.length() * closest.length();
		vec2 distance = closest - sphere->getPosition();
		float radius = sphere->getRadius();

		float distanceMagnitude = dot(distance, distance);

		if (distanceMagnitude < radius * radius)
		{
			cout << "Collided with box to Sphere" << endl;
			
			/*float overlap = (length(distance) + radius) - distanceMagnitude;

			float totalMass = sphere->getMass() + box->getMass();

			vec2 spherePos = sphere->getPosition() - normalize(normal) * (sphere->getMass() / totalMass) * overlap;
			vec2 boxPos = box->getPosition() + normalize(normal) * (box->getMass() / totalMass) * overlap;

			sphere->setPos(spherePos);
			box->setPos(boxPos);*/

			sphere->resolveCollision(box);
			box->resolveCollision(sphere);
			return true;
		}

	}
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2Box(obj2, obj1);
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane *plane = dynamic_cast<Plane*> (obj1);
	Box *box = dynamic_cast<Box*> (obj2);

	if (plane != nullptr && box != nullptr)
	{
		vec2 normal = plane->getNormal();
		//vec2 boxBL = box->getPosition();
		//vec2 boxBR = box->getPosition() + vec2(box->getLength(), 0);
		//vec2 boxTL = box->getPosition() + vec2(0, box->getHeight());
		//vec2 boxTR = box->getPosition() + vec2(box->getLength(), box->getHeight());
		vec2 boxBL = box->getPosition() - vec2(box->getLength(), box->getHeight());
		vec2 boxBR = box->getPosition() + vec2(box->getLength(), -1.0f*(box->getHeight()));
		vec2 boxTL = box->getPosition() + vec2(-1.0f*(box->getLength()), box->getHeight());
		vec2 boxTR = box->getPosition() + vec2(box->getLength(), box->getHeight());

		if (dot(normal, boxBL) - plane->getDistance() < 0 ||
			dot(normal, boxBR) - plane->getDistance() < 0 ||
			dot(normal, boxTL) - plane->getDistance() < 0 ||
			dot(normal, boxTR) - plane->getDistance() < 0)
		{
			if (dot(normal, boxBL) - plane->getDistance() < 0)
				overlap = dot(normal, boxBL) - plane->getDistance();

			else if (dot(normal, boxBR) - plane->getDistance() < 0)
				overlap = dot(normal, boxBR) - plane->getDistance();

			else if (dot(normal, boxTL) - plane->getDistance() < 0)
				overlap = dot(normal, boxTL) - plane->getDistance();

			else if (dot(normal, boxTR) - plane->getDistance() < 0)
				overlap = dot(normal, boxTR) - plane->getDistance();

			vec2 force = plane->resolveCollision(box);

			vec2 position = box->getPosition();
			position -= (overlap) * (0.5f) * (normal);

			box->setPos(position);
			box->applyForce(force);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return plane2Box(obj2, obj1);
}


bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box *box1 = dynamic_cast<Box*> (obj2);
	Box *box2 = dynamic_cast<Box*> (obj1);

	if (box1 != nullptr  && box2 != nullptr)
	{
		vec2 box1ColliderMax = box1->getPosition() + vec2(box1->getLength(), box1->getHeight());
		vec2 box1ColliderMin = box1->getPosition() - vec2(box1->getLength(), box1->getHeight());

		vec2 box2ColliderMax = box2->getPosition() + vec2(box2->getLength(),box2->getHeight());
		vec2 box2ColliderMin = box2->getPosition() - vec2(box2->getLength(), box2->getHeight());

		if (!(box1ColliderMax.x < box2ColliderMin.x ||
			  box2ColliderMax.x < box1ColliderMin.x ||
			  box1ColliderMax.y < box2ColliderMin.y ||
			  box2ColliderMax.y < box1ColliderMin.y))
		{
			
			box1->resolveCollision(box2);
			cout << "Collided AABB" << endl;
			return true;
		}
	}
	return false;
}

vec2 clamping(const vec2 vector, const vec2 & min, const vec2 & max)
{
	vec2 temp = vec2(0,0);
	if (vector.x < min.x)
		temp.x = min.x;

	if (vector.y < min.y)
		temp.y = min.y;

	if (vector.x > max.x)
		temp.x = max.x;

	if (vector.y > max.y)
		temp.y = max.y;

	return temp;
	
}