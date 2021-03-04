#include "EnemyTrigger.h"
#include "ECS.h"
#include "Player.h"

void EnemyTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void EnemyTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	//push player back
	auto& position = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]);
	auto& player = ECS::GetComponent<Player>(m_targetEntities[0]);

	auto& ghost = ECS::GetComponent<CanDamage>(m_triggerEntity);
	if (ghost.m_candamage && !ghost.m_stun)
	{
		//take health - 5
		MainEntities::Health(MainEntities::Health() - 5);
		auto direction = ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->GetLinearVelocity();
		direction *= -0.5f;
		if (player.m_facing == 0)
		{
			if (position.GetPosition().x + 60 >= 130)
			{
				ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(150, position.GetPosition().y), true);
			}
			else
			{
				ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(position.GetPosition().x + 60, position.GetPosition().y - 5), true);
			}
			
			//ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
		else
		{
			if (position.GetPosition().x - 60 <= -120)
			{
				ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(-100, position.GetPosition().y), true);
			}
			else
			{
				ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).SetPosition(b2Vec2(position.GetPosition().x - 60, position.GetPosition().y - 5), true);
			}
			
			//ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->SetLinearVelocity(b2Vec2(0, 0));
			ECS::GetComponent<PhysicsBody>(m_targetEntities[0]).GetBody()->SetLinearVelocity(direction);

		}
	}
}

void EnemyTrigger::OnExit()
{
	Trigger::OnExit();
}
