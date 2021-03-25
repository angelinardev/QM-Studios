#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\
	
	//Idle Left
	m_animController->AddAnimation(animations["IDLELEFT"].get<Animation>()); //0
	m_animController->AddAnimation(animations["IDLERIGHT"].get<Animation>()); //1
	//walk
	m_animController->AddAnimation(animations["WALKLEFT"].get<Animation>()); //2
	m_animController->AddAnimation(animations["WALKRIGHT"].get<Animation>()); //3
	//dashing
	m_animController->AddAnimation(animations["DASHLEFT"].get<Animation>()); //4
	m_animController->AddAnimation(animations["DASHRIGHT"].get<Animation>()); //5
	//attack
	m_animController->AddAnimation(animations["ATKLEFT"].get<Animation>()); //6
	m_animController->AddAnimation(animations["ATKRIGHT"].get<Animation>()); //7
	//wolf (reversed)
	m_animController->AddAnimation(animations["WLFIDLERIGHT"].get<Animation>()); //9
	m_animController->AddAnimation(animations["WLFIDLELEFT"].get<Animation>()); //8
	m_animController->AddAnimation(animations["WLFWALKRIGHT"].get<Animation>()); //11

	m_animController->AddAnimation(animations["WLFWALKLEFT"].get<Animation>()); //10
	
	//Set Default Animation
	m_animController->SetActiveAnim(1);


}
void Player::ReassignComponents(AnimationController* controller, Sprite* sprite)
{
	m_animController = controller;
	m_sprite = sprite;
	
}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;

	
		if (Input::GetKey(Key::A) || Input::GetKey(Key::LeftArrow))
		{
			
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D) || Input::GetKey(Key::RightArrow))
		{
			
			m_facing = RIGHT;
			m_moving = true;
		}

	/*if (Input::GetKeyDown(Key::Space))
	{
		m_moving = false;

		m_jump = true;
		m_locked = true;
	}*/
	if (Input::GetKeyDown(Key::Shift))
	{
		//m_moving = false;
		//m_dash = true;
		//m_locked = true;
	}
	if (Input::GetKey(Key::X))
	{
		/*m_attack = true;
		m_locked = true;
		m_moving = false;*/
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}
	//else if (m_jump)
	//{
	//	activeAnimation = DASH;

	//	//Check if the attack animation is done
	//	if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
	//	{
	//		//Will auto set to idle
	//		m_locked = false;
	//		m_jump = false;
	//		//Resets the attack animation
	//		m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

	//		activeAnimation = IDLE;
	//	}
	//}
	else if (m_dash)
	{
		activeAnimation = DASH;

		//Check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			m_locked = false;
			m_dash = false;
			//Resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	else if (m_attack) //attacked animation?
	{
		if (ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).m_canJump)
		{
			activeAnimation = ATTACK;

			if (m_facing == RIGHT)
			{
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetCenterOffset(vec2(20, -4.f));
			}
			else
			{
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetCenterOffset(vec2(-20, -4.f));
			}
			//Check if the attack animation is done
			if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
			{
				//Will auto set to idle
				m_locked = false;
				m_attack = false;
				//Resets the attack animation
				m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

				activeAnimation = IDLE;
				//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).ScaleBody(1/2, 0);
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetCenterOffset(vec2(0.f, -4.f));

			}
		}

	}
	else
	{
		activeAnimation = IDLE;
		
	}
	auto& power = ECS::GetComponent<Player_Power>(MainEntities::MainPlayer());
	if (power.m_power[1] && power.m_power[0]) //using powers
	{
		SetActiveAnimation(activeAnimation + (int)m_facing+ WOLF);
	}
	else
	{
		SetActiveAnimation(activeAnimation + (int)m_facing);
	}

	
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
