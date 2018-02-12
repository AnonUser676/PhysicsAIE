#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

vec2 clamping(const vec2 vector, const vec2 & min, const vec2 & max);

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
		vec2 temp;
		temp.x = sphere1->getPosition().x - sphere2->getPosition().x;
		temp.y = sphere1->getPosition().y - sphere2->getPosition().y;

		float distance;
		distance = sqrtf((temp.x * temp.x) + (temp.y * temp.y));

		float combinedRadius = (sphere1->getRadius() * sphere2->getRadius()) * (sphere1->getRadius() * sphere2->getRadius());

		if (distance <= combinedRadius)
		{
			//sphere1->setVelocity(vec2(0, 0));
			//sphere2->setVelocity(vec2(0, 0));
			cout << "Collided " << rand() % 100 << endl;
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

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			//sphere->setVelocity(vec2(0, 0));
			cout << "Collided Sphere2Plane " << rand() % 100 << endl;
			return true;
		}
	}
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
    {
	Plane *plane = dynamic_cast<Plane*>(obj1);
	Sphere * sphere = dynamic_cast<Sphere*> (obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			//sphere->setVelocity(vec2(0, 0));
			cout << "Collided Plane2Sphere" << rand() % 100 << endl;
			
			return true;
		}
	}
	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box *box = dynamic_cast<Box*> (obj1);
	Sphere *sphere = dynamic_cast<Sphere*> (obj2);

	if (box != nullptr && sphere != nullptr)
	{
		vec2 boxColliderLT = box->getPosition() + vec2(box->getLength() / 2.0f, box->getHeight() / 2.0f);
		vec2 boxColliderBR = boxColliderLT * -1.0f;

		vec2 pos = sphere->getPosition();
		float rad = sphere->getRadius();

		vec2 pointA = clamping(pos, boxColliderBR, boxColliderLT);
		vec2 V = pointA - pos;

		float VMag = (sqrtf((V.x * V.x) + (V.y * V.y)))*(sqrtf((V.x * V.x) + (V.y * V.y)));

		if (VMag <= rad * rad)
		{
			cout << "Collide Box : Sphere" << endl;
			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box *box1 = dynamic_cast<Box*> (obj1);
	Box *box2 = dynamic_cast<Box*> (obj2);

	if (box1 != nullptr  && box2 != nullptr)
	{
		vec2 box1ColliderLT = box1->getPosition() + vec2(box1->getLength()/2.0f, box1->getHeight()/2.0f);
		vec2 box1ColliderBR = box1ColliderLT * -1.0f;

		vec2 box2ColliderLT = box2->getPosition() + vec2(box2->getLength() / 2.0f, box2->getHeight() / 2.0f);
		vec2 box2ColliderBR = box2ColliderLT * -1.0f;

		if (box1ColliderBR.x > box2ColliderLT.x &&
			box1ColliderBR.y > box2ColliderLT.y &&
			box1ColliderLT.x < box2ColliderBR.x &&
			box1ColliderLT.y < box2ColliderBR.y)
		{
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