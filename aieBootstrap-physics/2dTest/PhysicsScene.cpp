#include "PhysicsScene.h"

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