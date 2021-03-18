#include "CanDamage.h"
#include "Player.h"

void CanDamage::InitBody(PhysicsBody b, AnimationController* a)
{
	body = b;
	anims = a;
	
}

void CanDamage::set_player(int p)
{
	player = p;
}

void CanDamage::Walk()
{
	auto& playerX = ECS::GetComponent<PhysicsBody>(player).GetBody()->GetPosition().x;
	auto& selfX = body.GetBody()->GetPosition().x;
	b2Vec2 vel = b2Vec2(body.GetBody()->GetLinearVelocity());
	float desireVel = vel.x;

	//enemy is within 100 of player
	if (selfX > playerX - 120 && selfX < playerX + 120)
	{
		if (playerX > selfX) //move right
		{
			desireVel = b2Max(vel.x + 2.5f, 25.f);
			//animation here
			//anims->SetActiveAnim(1);
			facing = 1;
			
		}
		else
		{
			desireVel = b2Min(vel.x - 2.f, -25.f);
			//anims->SetActiveAnim(0);
			facing = 0;

		}
		moving = true;
	}
	else //walk cycle/idle
	{
		moving = false;
		if (facing == 0)
		{
			//anims.SetActiveAnim(3);
		}
		else
		{
			//anims.SetActiveAnim(4);
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
	//MainEntities::Health(MainEntities::Health() - 25);
	ECS::GetComponent<CanJump>(player).hp -= 25;
	auto& player_b = ECS::GetComponent<PhysicsBody>(player);
	auto& selfX = body.GetBody()->GetPosition().x;
	if (player_b.GetBody()->GetPosition().x > selfX) //move right
	{
		//anims.SetActiveAnim(0); //attack animation for enemy
		//player hurt animation

		//lock animation for player
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_locked = true;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_moving = false;
		//knock back
		player_b.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(10000000, 0), true);
	}
	else
	{
		//anims.SetActiveAnim(0); //attack animation for enemy
		//player hurt animation

		//lock animation for player
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_locked = true;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).m_moving = false;
		//knock back
		player_b.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-10000000, 0), true);
	}
	

}

void CanDamage::Jump()
{
	//move upwards
	body.GetBody()->SetLinearVelocity(b2Vec2(body.GetBody()->GetLinearVelocity().x, 1600000000));
}
