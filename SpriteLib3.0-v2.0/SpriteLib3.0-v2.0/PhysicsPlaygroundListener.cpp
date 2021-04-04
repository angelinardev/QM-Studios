#include "PhysicsPlaygroundListener.h"
#include "Scene.h"
#include "ECS.h"
#include "BackEnd.h"

PhysicsPlaygroundListener::PhysicsPlaygroundListener()
	: b2ContactListener()
{

}

void PhysicsPlaygroundListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND)|| (filterA.categoryBits == PLAYER && filterB.categoryBits == OBJECTS)||(filterB.categoryBits == PLAYER && filterA.categoryBits == OBJECTS))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = true;
		}
	}
	//rope collisions
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == HEXAGON) || (filterB.categoryBits == PLAYER && filterA.categoryBits == HEXAGON))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).can_swing = true;
			ECS::GetComponent<Swing>((int)fixtureB->GetBody()->GetUserData()).m_swing = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).can_swing = true;
			ECS::GetComponent<Swing>((int)fixtureA->GetBody()->GetUserData()).m_swing = true;
		}
		
	}


	//enemy collisions
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == ENEMY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == ENEMY))
	{
		
		if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<CanDamage>((int)fixtureA->GetBody()->GetUserData()).Attack();
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<CanDamage>((int)fixtureB->GetBody()->GetUserData()).Attack();
		}
	}
	//enemy jump
	if ((filterA.categoryBits == ENVIRONMENT && filterB.categoryBits == ENEMY) || (filterB.categoryBits == ENVIRONMENT && filterA.categoryBits == ENEMY))
	{
		if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<CanDamage>((int)fixtureA->GetBody()->GetUserData()).Jump();
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<CanDamage>((int)fixtureB->GetBody()->GetUserData()).Jump();
		}
	}
}


void PhysicsPlaygroundListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();
	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND) || (filterA.categoryBits == PLAYER && filterB.categoryBits == OBJECTS) || (filterB.categoryBits == PLAYER && filterA.categoryBits == OBJECTS))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = false;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = false;
		}
	}
	//rope collisions
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == HEXAGON) || (filterB.categoryBits == PLAYER && filterA.categoryBits == HEXAGON))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).can_swing = false;
			ECS::GetComponent<Swing>((int)fixtureB->GetBody()->GetUserData()).m_swing = false;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).can_swing = false;
			ECS::GetComponent<Swing>((int)fixtureA->GetBody()->GetUserData()).m_swing = false;
		}

	}

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}
}

void PhysicsPlaygroundListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void PhysicsPlaygroundListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
