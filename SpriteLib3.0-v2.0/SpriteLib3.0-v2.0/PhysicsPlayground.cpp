#include "PhysicsPlayground.h"
#include "Utilities.h"

#include <random>
#include <cmath>

static int healthBar = 0;
static int healthBarBack = 0;
static int ghostBar = 0;
static int ghostBarBack = 0;
static std::vector<int> ghostCount;
static int uiBG = 0;

static int pcount = 0;
HealthBar hb;

int PhysicsPlayground::ChangeScene()
{

	return selection;

}

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//initialize the health
	MainEntities::Health(100);
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	//EffectManager::CreateEffect(EffectType::Sepia, windowWidth, windowHeight);
	

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "back_tut.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1440, 194);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(315.f, 0.f, 0.f));
	}
	
	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<Player_Power>(entity);

		//Sets up the components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-380.f), float32(-60.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT| ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT|ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		
	}

	//SetUp Invisible Wall at the beginning
	BoxMaker(75, 20, -410.f, -70.f, 90, 0);
	
	//Setup spawning static Platform
	BoxMaker(198, 5, -310.f, -80.f, 0, 0,2);
	
	//Setup Downward log Log 
	BoxMaker(75, 15, -223.f, -70.f, 160, 0, 0.1);
		
	//Setup Static platform after log
	BoxMaker(75, 8, -185.f, -85.f, 5, 0,2);

	//Setup for the first rock
	BoxMaker(40, 10, -134.f, -55.f, 35, 0,0.10);
	//BoxMaker(5, 4, -115.f, -45.f, 0, 0);

	//Setup for the second rock
	BoxMaker(35, 3, -95.f, -30.f, 30, 0, 0.2);
	BoxMaker(10, 4, -74.f, -23.f, 0, 0,2);

	//Setup Static after second rock
	BoxMaker(60, 8, -55, -75, 0, 0,2);
	BoxMaker(30, 50, -90.f, -58.f, 90, 0);

	//Setup for the third rock
	BoxMaker(15, 3, -10.f, -55.f, 30, 0, 0.2);
	BoxMaker(20, 3, 5.f, -52.f, 0, 0);

		
	//Setup Static after third rock
	BoxMaker(40, 8, 30, -75, 0, 0);

	//Setup for the fourth rock
	BoxMaker(30, 3, 70.f, -47.f, 30, 0,0.2);
	BoxMaker(30, 3, 95.f, -41.f, 0, 0);

	//Setup a block for under the rock
	BoxMaker(30, 25, 95.f, -75.f, 90, 1);

	//Setup for path after jump
	BoxMaker(43, 2, 272.f, -65.f, 0, 0,2);

	//Set up for tree stump
	BoxMaker(15, 3, 310.f, -35.f, 0, 0);

	//Set up log after tree stump
	BoxMaker(45, 3, 350.f, -25.f, 165, 0,2);
	BoxMaker(30, 3, 365.f, -20.f, 25, 0,0.2);
	BoxMaker(20, 3, 380.f, -30.f, 0, 0,2);
	
	//Setup stump after log
	BoxMaker(30, 3, 410.f, -50.f, 140, 0,0.2);
	
	//Setup Static platform
	BoxMaker(80, 3, 465.f, -60.f, 0, 0,2);

	//Setup Top Platform
	BoxMaker(60, 3, 540.f, -25.f, 0, 0,2);
	BoxMaker(40, 3, 520.f, -50.f, 105, 0,2);
	BoxMaker(15, 3, 575.f, -30.f, 145, 0,0.1);
	BoxMaker(15, 3, 590.f, -30.f, 90, 0,0.2);
	BoxMaker(30, 3, 608.f, -25.f, 170, 0,0.2);
	BoxMaker(30, 3, 617.f, -25.f, 170, 0,0.2);

	//Setup wood twist
	BoxMaker(30, 3, 650.f, -27.f, 15, 0);
	BoxMaker(30, 3, 668.f, -20.f, 45, 0,0.1);

	//Setup Platform after wood twist
	BoxMaker(25, 3, 690.f, -10.f, 0, 0,2);

	//Setup Down to last platfrom
	BoxMaker(75, 4, 720.f, -40.f, 125, 0);

	//Setup Last Platform
	BoxMaker(170, 4, 840.f, -75.f, 0, 0,2);


	//Add components

	/* Invisibility platform example
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "boxsprite.jpg";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-10.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		test_e1 = entity;
	}

	*/

	/*
	//testing pickup
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "Book.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PickupTrigger(1); //first powerup
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(50), float32(0));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	*/
	{//Health bar (green)

		healthBar = Scene::createHealthBar();
	}

	//{//health bar back (brown)

	//	healthBarBack = Scene::createHealthBarBack();
	//}
	//UI background
	{
		uiBG = Scene::createUIBack();
	}
	//powers
	{
		pcount = Scene::createP();
	}
	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	if (!dashcooldown) {
		dash_timer = (clock() - dashtime) / CLOCKS_PER_SEC;
		if (dash_timer >= cooldown) {
			can_dash = true;
			dashcooldown = true;
		}
	}
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (player.GetBody()->GetLinearVelocity().y < 0 && !canJump.m_canJump)//peak of jump, position needs to be relative to the ground
	{
		player.SetGravityScale(4.5);
		jspeed += 0.8;
		if (jspeed > 6)
		{
			jspeed = 6;
		}
		player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -8000.f * jspeed), true);
		
		//player.GetBody()->SetLinearVelocity(b2Vec2(player.GetBody()->GetLinearVelocity().x, -1900*jspeed));
	}
		else {
			jspeed = 0;
		}

	
	hb.UpdateHealthBar(healthBar,uiBG);
	hb.UpdatePowers(pcount);

	//for jumping under the pit
	if (player.GetPosition().y <= -90 && player.GetPosition().x <=800)
	{
		//bring player back to position before jump
		player.GetBody()->SetTransform(b2Vec2(85, 20), 0);
	}
	//check for the lake
	if (player.GetPosition().x >= 900 && player.GetPosition().y <= -90)
	{
		selection = 2; //next scene
	}

	//hb.UpdateGhostCounter(ghostCount, ghostBar, ghostBarBack);

	//update invisibility
	//ECS::GetComponent<Invisibility>(test_e1).update_invisible();

	////setup animation component again so the player doesnt lose their animations
	//ECS::GetComponent<Player>(MainEntities::MainPlayer()).ReassignComponents(
	//	&ECS::GetComponent<AnimationController>(MainEntities::MainPlayer()),
	//	&ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer())
	//);
}

void PhysicsPlayground::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}

}

void PhysicsPlayground::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}
	
	ImGui::End();
}

void PhysicsPlayground::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();
	
	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();
	
	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void PhysicsPlayground::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0,1), ImVec2(1,0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();

	ImGui::End();
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	b2Vec2 vel = b2Vec2(0.f, 0.f);

	b2Vec2 pvel = player.GetBody()->GetLinearVelocity();
	if (Input::GetKey(Key::A)) //left
	{
		facing = 0;
		speed += 0.5; //first slope
		if (speed > 3) //max speed
		{
			speed = 3;
		}
		
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
		
		
	}
	else if (Input::GetKey(Key::D)) //right
	{
		facing = 1;

		speed += 0.5; //first slope
		if (speed > 3) //max speed
		{
			speed = 3;
		}
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
		
	}
	else
	{
		if (speed > 0)
		{
			speed -= 1; //second slope
		}
		if (speed <= 0) { //ensure speed isnt negative
			speed = 0;
		}
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& power = ECS::GetComponent<Player_Power>(MainEntities::MainPlayer());

	auto& vel = player.GetBody()->GetLinearVelocity();
	auto& pos = player.GetBody()->GetPosition();

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (Input::GetKeyDown(Key::One)) //jump higher ability
	{
		if (MainEntities::Powerups()[0])
		{
			power.m_power[0] = !power.m_power[0]; //reverses choice
		}
	}
	/*
	if (Input::GetKeyDown(Key::Two)) //vision
	{
		if (MainEntities::Powerups()[1])
		{
			power.m_power[1] = !power.m_power[1]; //reverses choice
			//manually change values
			ECS::GetComponent<Invisibility>(test_e1).is_invisible = !ECS::GetComponent<Invisibility>(test_e1).is_invisible;
		}
	}
	*/
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			if (power.m_power[0]) //jump higher
			{
				player.SetGravityScale(1.f);
			}
			else
			{
				player.SetGravityScale(2.f);
			}
			theta = 1;
			player.GetBody()->SetLinearVelocity(b2Vec2(vel.x, 1600000000));
			
			//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 1600000000.f), true);
			canJump.m_canJump = false;
		}
		dash_timer = 0;
	}
	

	//dash
	if (Input::GetKeyDown(Key::Shift))
	{
		if (canJump.m_canJump && can_dash) //ground dash
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(0, vel.y));
			if (facing == 0) //left
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-400000.f * 1000, 0.f), true);
				//player.GetBody()->SetTransform(b2Vec2(pos.x - 30, pos.y), 0);
				player.GetBody()->SetLinearVelocity(b2Vec2(-1000000, vel.y));
				can_dash = false;
			}
			else if (facing == 1)
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(400000.f * 1000, 0.f), true);
				//player.GetBody()->SetTransform(b2Vec2(pos.x + 30, pos.y), 0);
				player.GetBody()->SetLinearVelocity(b2Vec2(1000000, vel.y));
				can_dash = false;
			}
			
		}
		else if (dash_timer >= 2.5) //player can dash once in the air
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(0, vel.y));
			if (facing == 0) //left
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-400000.f * 1000, 0.f), true);
				player.GetBody()->SetTransform(b2Vec2(pos.x - 30, pos.y), 0);
			}
			else if (facing == 1)
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(400000.f * 1000, 0.f), true);
				player.GetBody()->SetTransform(b2Vec2(pos.x + 30, pos.y), 0);
			}

			dash_timer = 0;
		}

		else if (!canJump.m_canJump && can_dash) //player can dash once in the air
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(0, vel.y));
			if (facing == 0) //left
			{
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-400000.f * 1000, 0.f), true);
				player.GetBody()->SetTransform(b2Vec2(pos.x - 30, pos.y), 0);
				player.GetBody()->SetLinearVelocity(b2Vec2(-1000000, vel.y));
				//can_dash = false;
			}
			else if (facing == 1)
			{
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(400000.f * 1000, 0.f), true);
				player.GetBody()->SetTransform(b2Vec2(pos.x + 30, pos.y), 0);
				//player.GetBody()->SetLinearVelocity(b2Vec2(1000000, vel.y));
				can_dash = false;
			}
		}
	}
	//dash cooldown
	if (!can_dash && dashcooldown)
	{
		dashtime = clock();
		dashcooldown = false;
	}
	
	
}

void PhysicsPlayground::KeyboardUp()
{
	

}
