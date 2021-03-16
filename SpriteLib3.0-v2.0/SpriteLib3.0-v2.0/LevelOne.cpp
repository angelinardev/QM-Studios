#include "LevelOne.h"
#include "Utilities.h"
#include "ToneFire/ToneFire.h"

#include <random>
#include <cmath>

int LevelOne::ChangeScene()
{
	return selection;

}

LevelOne::LevelOne(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);


}
void LevelOne::InitScene(float windowWidth, float windowHeight)
{
	
	selection = -1;
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

	
	ECS::SetIsMainPlayer(p_entity, true);
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));
	//Sound.Play();

	inputS.open("HP.txt");
	int var;
	if (inputS.is_open())
	{
		inputS >> var;
		ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).hp = var;
	}
	inputS.close();

	enemy1 = true;

	//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetAwake(true);
	//ECS::GetComponent<PhysicsBody>(enemy).GetBody()->SetAwake(true);
}
void LevelOne::InitTexture()
{

	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "back_tutdeath.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1790, 340);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(317.f, -5.f, 1.f));
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
		std::string fileName = "lvl1back.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1900, 300);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(315.f, 0.f, 2.f));
	}

	//enemy
	
	{

		auto entity = ECS::CreateEntity();
		enemy = entity;
		
		//Add components  
	
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<CanDamage>(entity);

		ECS::GetComponent<CanDamage>(entity).m_candamage = true;

		//Sets up the components  

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
		//Sets up the components  
		std::string fileName = "spritesheets/neville.png";
		std::string animations = "Boss1.json";

		animController.InitUVs(fileName);
		nlohmann::json animations2 = File::LoadJSON(animations);
		animController.AddAnimation(animations2["Boss1Idle"].get<Animation>());
		animController.AddAnimation(animations2["Boss1Fingermove"].get<Animation>());
		animController.AddAnimation(animations2["Boss1Yawn"].get<Animation>());
		animController.AddAnimation(animations2["Boss1Suck"].get<Animation>());
		animController.AddAnimation(animations2["Boss1Suck2"].get<Animation>());//face right?
		animController.SetActiveAnim(0);

		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		//ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		//ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));
	
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30, true, &animController);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-300.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, OBJECTS | ENVIRONMENT | PLAYER | GROUND, 0.5f, 3.f);
	

		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetGravityScale(2.5f);
		tempPhsBody.SetFixedRotation(true);
		ECS::GetComponent<CanDamage>(entity).InitBody(tempPhsBody, animController);
		//tempSpr.SetTransparency(0);
	}
	

	//SetUp Invisible Wall at the beginning
	BoxMaker(1000, 20, -550.f, 0.f, 90, 0);

	//Setup spawning static Platform
	BoxMaker(210, 5, -440.f, -15.f, 0, 0, 6);

	////Setup first invis platform
	//{
	//	auto entity = ECS::CreateEntity();

	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);
	//	std::string fileName = "boxsprite.jpg";
	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(-330.f), float32(-10.f));


	//	tempBody = m_physicsWorld->CreateBody(&tempDef);
	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	ECS::AttachComponent<Invisibility>(entity);
	//	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	//	invis1 = entity;
	//}

	////Setup second invis
	//{
	//	auto entity = ECS::CreateEntity();

	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);
	//	std::string fileName = "boxsprite.jpg";
	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(-315.f), float32(0.f));


	//	tempBody = m_physicsWorld->CreateBody(&tempDef);
	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	ECS::AttachComponent<Invisibility>(entity);
	//	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	//	invis2 = entity;
	//}

	////Setup third invis
	//{
	//	auto entity = ECS::CreateEntity();

	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);
	//	std::string fileName = "boxsprite.jpg";
	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(45.f), float32(15.f));


	//	tempBody = m_physicsWorld->CreateBody(&tempDef);
	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	ECS::AttachComponent<Invisibility>(entity);
	//	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	//	invis3 = entity;
	//}

	////Setup fourth invis platform
	//{
	//	auto entity = ECS::CreateEntity();

	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);
	//	std::string fileName = "boxsprite.jpg";
	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(55.f), float32(5.f));


	//	tempBody = m_physicsWorld->CreateBody(&tempDef);
	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	ECS::AttachComponent<Invisibility>(entity);
	//	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	//	invis1 = entity;
	//}

	////Setup Downward log Log 
	//BoxMaker(75, 15, -223.f, -70.f, 157, 1, 0.1);

	////Setup Static platform after log
	//BoxMaker(75, 8, -185.f, -80.f, 0, 1, 2);

	////Setup for the first rock
	//BoxMaker(40, 10, -134.f, -58.f, 35, 1, 0.10);
	////BoxMaker(5, 4, -115.f, -45.f, 0, 0);

	////Setup for the second rock
	//BoxMaker(40, 3, -95.f, -31.f, 30, 0, 0.2);
	//BoxMaker(10, 4, -73.5f, -21.8f, 0, 0, 2);

	////Setup Static after second rock
	//BoxMaker(60, 8, -55, -75, 0, 0, 2);
	//EnviroMaker(30, 50, -94.f, -58.f, 90, 0);
	//testing pickup
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

	////Setup for the third rock
	//BoxMaker(25, 3, -13.f, -57.f, 27, 0, 0.2);
	//BoxMaker(20, 3, 7.f, -51.8f, 0, 0);
		//Sets up components
		std::string fileName = "page.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PickupTrigger(2); //first powerup
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


	////Setup Static after third rock
	//BoxMaker(40, 8, 30, -75, 0, 0);
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-375), float32(0));
	////Setup for the fourth rock
	//BoxMaker(30, 3, 70.f, -47.f, 24, 0, 0.2);
	//BoxMaker(25, 3, 96.f, -41.f, 0, 0);

		tempBody = m_physicsWorld->CreateBody(&tempDef);
	////Setup a block for under the rock
	//BoxMaker(30, 60, 95.f, -75.f, 0, 0);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	////Setup for path after jump
	//BoxMaker(43, 2, 272.f, -65.f, 0, 0, 2);
	////under blocks
	//EnviroMaker(30, 90, 350.f, -57.f, 90, 0);
	////EnviroMaker(30, 25, 390.f, -57.f, 90, 0);

	////Set up for tree stump
	//BoxMaker(15, 3, 310.f, -35.f, 0, 0);

	////Set up log after tree stump
	//BoxMaker(45, 3, 350.f, -25.f, 165, 0, 2);
	//BoxMaker(30, 3, 365.f, -20.f, 25, 0, 0.2);
	//BoxMaker(10, 3, 383.f, -10.f, 43, 0, 0.2);
	//BoxMaker(20, 3, 380.f, -30.f, 0, 0, 2);

	////Setup stump after log
	//BoxMaker(30, 3, 410.f, -50.f, 140, 0, 0.2);

	////Setup Static platform
	//BoxMaker(80, 3, 465.f, -60.f, 0, 0, 2);

	////Setup Top Platform
	//BoxMaker(60, 3, 540.f, -25.f, 0, 0, 2);
	//BoxMaker(40, 3, 520.f, -50.f, 105, 0, 2);

	//BoxMaker(15, 3, 575.f, -28.f, 145, 0, 0.1);
	//BoxMaker(18, 3, 590.f, -29.8f, 90, 0, 0.2);
	//BoxMaker(45, 3, 613.f, -26.f, 170, 0, 0.2);
	////BoxMaker(30, 3, 617.f, -25.f, 170, 0,0.2);



	////Setup wood twist
	//BoxMaker(30, 3, 650.f, -27.f, 15, 0);
	//BoxMaker(30, 3, 668.f, -20.f, 45, 0, 0.1);

	////Setup Platform after wood twist
	//BoxMaker(25, 3, 690.f, -10.f, 0, 0, 2);

	////Setup Down to last platfrom
	//BoxMaker(75, 4, 720.f, -40.f, 125, 0);
	////block underneath to block
	//EnviroMaker(200, 40, 615, -50.f, 0, 0);


	////Setup Last Platform
	//BoxMaker(170, 4, 840.f, -75.f, 0, 0, 2);

	
	{//Health bar (green)

		healthBar = Scene::createHealthBar();
	}

	//{//health bar back (brown)

	//	healthBarBack = Scene::createHealthBarBack();
	//}
	//UI background
	{
		//uiBG = Scene::createUIBack();
	}
	//powers
	{
		pcount = Scene::createP();
	}

	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		p_entity = entity;


		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<Player_Power>(entity);
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/charspriteupside.png";
		std::string animations = "Char.json";
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 50, 40, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 38.f;
		float shrinkY = 6.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-530.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);


		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER |ENVIRONMENT, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -4.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | PTRIGGER |GROUND, 0.4f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT|ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		ECS::GetComponent<AnimationController>(entity).SetActiveAnim(1); //right
		tempPhsBody.SetGravityScale(2.5f);
	}

}

void LevelOne::Update()
{
	//Fmod.Update();

	auto& player2 = ECS::GetComponent<Player>(p_entity);
	player2.Update();

	//if (enemy1)
	//{
	//	auto& enemy_c = ECS::GetComponent<CanDamage>(enemy);
	//	enemy_c.Walk();
	//	//check if enemy is dead
	//	if (enemy_c.hp <= 0)
	//	{
	//		PhysicsBody::m_bodiesToDelete.push_back(enemy);
	//		enemy1 = false;
	//	}
	//}
	
	ECS::GetComponent<Invisibility>(invis1).update_invisible();
	ECS::GetComponent<Invisibility>(invis2).update_invisible();
	ECS::GetComponent<Invisibility>(invis3).update_invisible();
	ECS::GetComponent<Invisibility>(invis4).update_invisible();
	//ECS::GetComponent<Invisibility>(invis5).update_invisible();
	//ECS::GetComponent<Invisibility>(invis6).update_invisible();
	//ECS::GetComponent<Invisibility>(invis7).update_invisible();
	//ECS::GetComponent<Invisibility>(invis8).update_invisible();
	//ECS::GetComponent<Invisibility>(invis9).update_invisible();

	//auto& enemy_c = ECS::GetComponent<CanDamage>(enemy);
	//enemy_c.Walk();



	auto& dash = ECS::GetComponent<CanJump>(p_entity);
	//enemy checks
	//auto& ghost = ECS::GetComponent<PhysicsBody>(ghost1);
	//auto& c_ghost = ECS::GetComponent<CanDamage>(ghost1);
	//auto& anims = ECS::GetComponent<AnimationController>(ghost1);
	//auto& c_ghost2 = ECS::GetComponent<CanDamage>(ghost2);
	//auto& ghost_2 = ECS::GetComponent<PhysicsBody>(ghost2);
	//ghost_2.SetPosition(b2Vec2(ghost.GetBody()->GetWorldCenter()), false);
	//if (c_ghost.m_candamage) //default
	//{
	//	ghost.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
	//	startstuntime = clock();
	//	isyawn = false;
	//	//anims.SetActiveAnim(0); //default
	//}
	//else if (!c_ghost.m_stun) { //stunned
	//	float elapsedtime;
	//	float stuntime = 5.0f;
	//	//anims.SetActiveAnim(loop_anim);
	//	c_ghost2.m_candamage = false;
	//	isstunned = true;
	//	if (isstunned) {
	//		elapsedtime = (clock() - startstuntime) / CLOCKS_PER_SEC;

	//		if (elapsedtime >= stuntime) { //unstun
	//			c_ghost.m_candamage = true;
	//			c_ghost.m_stun = false;
	//			c_ghost2.m_candamage = true;
	//			c_ghost2.m_stun = false;
	//			//ghost.GetBody()->SetLinearVelocity(b2Vec2(15, 0));
	//			isstunned = false;
	//		}
	//	}
	//}

	if (dash.hp <= 0) //dying
	{
		selection = 2; //end screen? for now

	}
	if (!dashcooldown) {
		dash_timer = (clock() - dashtime) / CLOCKS_PER_SEC;
		if (dash_timer >= cooldown) {
			dash.can_dash = true;
			dashcooldown = true;
		}
	}
	auto& player = ECS::GetComponent<PhysicsBody>(p_entity);
	auto& canJump = ECS::GetComponent<CanJump>(p_entity);


	if (player.GetBody()->GetLinearVelocity().y < 0 && !canJump.m_canJump)//peak of jump, position needs to be relative to the ground
	{
		if (!jump_high)
		{
			player.SetGravityScale(5);
		}
		else
		{
			player.SetGravityScale(3);
		}
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
		if (!jump_high)
		{
			player.SetGravityScale(2.2);
		}
		else
		{
			player.SetGravityScale(1.5);
		}
	}


	hb.UpdateHealthBar(healthBar);
	hb.UpdatePowers(pcount);

	//setup animation component again so the player doesnt lose their animations
	player2.ReassignComponents(
		&ECS::GetComponent<AnimationController>(p_entity),
		&player, &ECS::GetComponent<Sprite>(p_entity));
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));


}

void LevelOne::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(p_entity);

	b2Vec2 vel = b2Vec2(0.f, 0.f);
	auto& canJump = ECS::GetComponent<CanJump>(p_entity);

	b2Vec2 pvel = player.GetBody()->GetLinearVelocity();

	if (Input::GetKey(Key::A)) //left
	{
		facing = 0;
		speed += 0.5; //first slope
		if (speed > 3) //max speed
		{
			speed = 3;
		}
		if (canJump.m_canJump)
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(-400000.f * speed, 0.f));

		}
		else
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
		}


	}
	else if (Input::GetKey(Key::D)) //right
	{
		facing = 1;

		speed += 0.5; //first slope
		if (speed > 3) //max speed
		{
			speed = 3;
		}
		if (canJump.m_canJump)
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(400000.f * speed, 0.f));

		}
		else
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
		}

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

void LevelOne::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(p_entity);
	auto& canJump = ECS::GetComponent<CanJump>(p_entity);
	auto& power = ECS::GetComponent<Player_Power>(p_entity);

	auto& vel = player.GetBody()->GetLinearVelocity();
	auto& pos = player.GetBody()->GetPosition();
	//auto& dash = ECS::GetComponent<CanJump>(p_entity);



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
	
	if (Input::GetKeyDown(Key::Two)) //vision
	{
		if (MainEntities::Powerups()[1])
		{
			power.m_power[1] = !power.m_power[1]; //reverses choice
			//manually change values
			ECS::GetComponent<Invisibility>(invis1).is_invisible = !ECS::GetComponent<Invisibility>(invis1).is_invisible;
			ECS::GetComponent<Invisibility>(invis2).is_invisible = !ECS::GetComponent<Invisibility>(invis2).is_invisible;
			ECS::GetComponent<Invisibility>(invis3).is_invisible = !ECS::GetComponent<Invisibility>(invis3).is_invisible;
			ECS::GetComponent<Invisibility>(invis4).is_invisible = !ECS::GetComponent<Invisibility>(invis4).is_invisible;
			//ECS::GetComponent<Invisibility>(invis5).is_invisible = !ECS::GetComponent<Invisibility>(invis5).is_invisible;
			//ECS::GetComponent<Invisibility>(invis6).is_invisible = !ECS::GetComponent<Invisibility>(invis6).is_invisible;
			//ECS::GetComponent<Invisibility>(invis7).is_invisible = !ECS::GetComponent<Invisibility>(invis7).is_invisible;
			//ECS::GetComponent<Invisibility>(invis8).is_invisible = !ECS::GetComponent<Invisibility>(invis8).is_invisible;
			//ECS::GetComponent<Invisibility>(invis9).is_invisible = !ECS::GetComponent<Invisibility>(invis9).is_invisible;
		}
	}
	
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			if (power.m_power[0]) //jump higher
			{
				player.SetGravityScale(1.5f);
				jump_high = true;
			}
			else
			{
				jump_high = false;
				player.SetGravityScale(2.2f);
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
		if (canJump.m_canJump && canJump.can_dash) //ground dash
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(0, vel.y));
			if (facing == 0) //left
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-400000.f * 1000, 0.f), true);
				//player.GetBody()->SetTransform(b2Vec2(pos.x - 30, pos.y), 0);
				player.GetBody()->SetLinearVelocity(b2Vec2(-1000000, vel.y));
				canJump.can_dash = false;
			}
			else if (facing == 1)
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(400000.f * 1000, 0.f), true);
				//player.GetBody()->SetTransform(b2Vec2(pos.x + 30, pos.y), 0);
				player.GetBody()->SetLinearVelocity(b2Vec2(1000000, vel.y));
				canJump.can_dash = false;
			}
			ECS::GetComponent<Player>(p_entity).m_dash = true;
			ECS::GetComponent<Player>(p_entity).m_locked = true;
			ECS::GetComponent<Player>(p_entity).m_moving = false;

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
			ECS::GetComponent<Player>(p_entity).m_dash = true;
			ECS::GetComponent<Player>(p_entity).m_locked = true;
			ECS::GetComponent<Player>(p_entity).m_moving = false;

			dash_timer = 0;
		}

		else if (!canJump.m_canJump && canJump.can_dash) //player can dash once in the air
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(0, vel.y));
			if (facing == 0) //left
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-400000.f * 1000, 0.f), true);
				player.GetBody()->SetTransform(b2Vec2(pos.x - 30, pos.y), 0);
				//player.GetBody()->SetLinearVelocity(b2Vec2(-1000000, vel.y));
				canJump.can_dash = false;
			}
			else if (facing == 1)
			{
				//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(400000.f * 1000, 0.f), true);
				player.GetBody()->SetTransform(b2Vec2(pos.x + 30, pos.y), 0);
				//player.GetBody()->SetLinearVelocity(b2Vec2(1000000, vel.y));
				canJump.can_dash = false;
			}
			ECS::GetComponent<Player>(p_entity).m_dash = true;
			ECS::GetComponent<Player>(p_entity).m_locked = true;
			ECS::GetComponent<Player>(p_entity).m_moving = false;
		}
	}
	//dash cooldown
	if (!canJump.can_dash && dashcooldown)
	{
		dashtime = clock();
		dashcooldown = false;
	}

}

void LevelOne::KeyboardUp()
{


}
