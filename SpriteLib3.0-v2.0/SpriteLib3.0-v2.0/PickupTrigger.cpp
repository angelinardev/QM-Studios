#include "PickupTrigger.h"
#include "ECS.h"
#include "Player.h"


PickupTrigger::PickupTrigger(int n)
{
	type = n;
	powers = MainEntities::Powerups();
}

void PickupTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void PickupTrigger::OnEnter()
{
	Trigger::OnEnter();
	powers[type - 1] = true;
	
	//switch (type)
	//{
	//	case 1: //jump higher
	//		if (!powers[0])
	//		{
	//			powers[0] = true;
	//		}

	//}
	MainEntities::Powerups(powers);

	//MainEntities::Health(MainEntities::Health() + 25); //recover health
	PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity); //deletes itself

	
}



void PickupTrigger::OnExit()
{
	Trigger::OnExit();


}
