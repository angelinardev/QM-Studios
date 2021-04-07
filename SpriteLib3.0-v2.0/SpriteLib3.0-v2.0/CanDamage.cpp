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
	//phealth = ECS::GetComponent<CanJump>(p).hp;
}

void CanDamage::Walk()
{
	auto& playerX = ECS::GetComponent<PhysicsBody>(player).GetBody()->GetPosition().x;
	auto& playerY = ECS::GetComponent<PhysicsBody>(player).GetBody()->GetPosition().y;
	auto& selfX = body.GetBody()->GetPosition().x;
	auto& selfY = body.GetBody()->GetPosition().y;
	b2Vec2 vel = b2Vec2(body.GetBody()->GetLinearVelocity());
	float desireVel = vel.x;

	//enemy is within 100 of player
	if ((selfX > playerX - 120 && selfX < playerX + 120) &&(selfY > playerY - 20 && selfY < playerY +20))
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
	//phealth -= 25;
	auto& player_b = ECS::GetComponent<PhysicsBody>(player);
	auto& selfX = body.GetBody()->GetPosition().x;
	attack = true;
	moving = false;
	if (player_b.GetBody()->GetPosition().x > selfX) //move right
	{
		//knock back
		player_b.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(10000000, 0), true);
	}
	else
	{
		//knock back
		player_b.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-10000000, 0), true);
	}
	

}

void CanDamage::Jump()
{
	//move upwards
	body.GetBody()->SetLinearVelocity(b2Vec2(body.GetBody()->GetLinearVelocity().x, 160000000000));
}
