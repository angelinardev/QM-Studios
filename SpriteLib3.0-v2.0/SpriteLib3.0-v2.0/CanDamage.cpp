#include "CanDamage.h"
#include "Player.h"

void CanDamage::InitBody(PhysicsBody& b, AnimationController& a)
{
	body = b;
	anims = a;
}

void CanDamage::Walk()
{
	auto& playerX = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition().x;
	auto& selfX = body.GetBody()->GetPosition().x;
	b2Vec2 vel = b2Vec2(body.GetBody()->GetLinearVelocity());
	float desireVel = vel.x;

	//enemy is within 100 of player
	if (selfX > playerX - 100 && selfX < playerX + 100)
	{
		if (playerX > selfX) //move right
		{
			desireVel = b2Max(vel.x + 2.5f, 20.f);
			//animation here
		}
		else
		{
			desireVel = b2Min(vel.x - 2.f, -20.f);
		}
	}
	//doesnt mean when not close
	float velC = desireVel - vel.x;
	float impulse = body.GetBody()->GetMass() * velC;
	//move
	body.GetBody()->ApplyLinearImpulse(b2Vec2(impulse, 0), body.GetBody()->GetWorldCenter(), true);

}

void CanDamage::Attack()
{
	//start timer
	//if timer allows:
	//remove health
	MainEntities::Health(MainEntities::Health() - 25);

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& selfX = body.GetBody()->GetPosition().x;
	if (player.GetBody()->GetPosition().x > selfX) //move right
	{
		//anims.SetActiveAnim(0); //attack animation for enemy
		//player hurt animation

		//lock animation for player
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_locked = true;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_moving = false;
		//knock back
		player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(10000000, 0), true);
	}
	else
	{
		//anims.SetActiveAnim(0); //attack animation for enemy
		//player hurt animation

		//lock animation for player
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_locked = true;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_moving = false;
		//knock back
		player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-10000000, 0), true);
	}
	

}
