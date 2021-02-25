#include "TutorialTrig.h"
#include "ECS.h"

TutorialTrig::TutorialTrig(int n)
{
	entity = n;
}

void TutorialTrig::OnTrigger()
{
	Trigger::OnTrigger();
}

void TutorialTrig::OnEnter()
{
	Trigger::OnEnter();
	ECS::GetComponent<Sprite>(entity).SetTransparency(1);
	PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity); //deletes itself
}

void TutorialTrig::OnExit()
{
	Trigger::OnExit();
}
