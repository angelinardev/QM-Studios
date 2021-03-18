#include "HealthTrigger.h"
#include "ECS.h"

void HealthTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void HealthTrigger::OnEnter()
{
	Trigger::OnEnter();
	ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).hp += 25; //recover health
	if (ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).hp > 100)
	{
		ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).hp = 100; //make sure it doesnt go above 100
	}
	PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity); //deletes itself
}

void HealthTrigger::OnExit()
{
	Trigger::OnExit();
}
