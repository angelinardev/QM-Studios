#include "LevelTwo.h"
#include "Utilities.h"
#include "ToneFire/ToneFire.h"
#include "Game.h"
#include <random>
#include <cmath>

int LevelTwo::ChangeScene()
{
	return selection;

}

LevelTwo::LevelTwo(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);


}
void LevelTwo::InitScene(float windowWidth, float windowHeight)
{
	//clear vector
	//enemies.clear();
	//alive.clear();

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
	Sound.SetVolume(100);
	Sound.Play();

	inputS.open("HP.txt");
	int var;
	if (inputS.is_open())
	{
		inputS >> var;
		ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).hp = var;
	}
	inputS.close();


	//setup enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		ECS::GetComponent<CanDamage>(enemies[i]).set_player(p_entity);
		alive.push_back(true);
	}
	ECS::GetComponent<AnimationController>(p_entity).SetActiveAnim(1); //face right

	//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetAwake(true);
	//ECS::GetComponent<PhysicsBody>(enemy).GetBody()->SetAwake(true);

}
void LevelTwo::InitTexture()
{
	is_done = true;
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	//{
	//	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//	//Creates entity
	//	auto entity = ECS::CreateEntity();

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);

	//	//Set up the components
	//	std::string fileName = "back_tutdeath.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1790, 340);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(317.f, -5.f, 1.f));
	//}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		tut1 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Attack.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-520.f, -20.f, 3.f));

	}

	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		tut2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Change to Wolf.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-350.f, 10.f, 5.f));
	}
	//tut sensor2
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "page.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new TutorialTrig(tut2); //first tutorial
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-380), float32(-15));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
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
		std::string fileName = "outlook.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 1500);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(315.f, 0.f, 2.f));
	}

	//enemy
	/*
	{

		auto entity = ECS::CreateEntity();
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
		std::string fileName = "spritesheets/scarecrow.png";
		std::string animations = "Scarecrow.json";

		animController.InitUVs(fileName);
		nlohmann::json animations2 = File::LoadJSON(animations);
		animController.AddAnimation(animations2["WALKLEFT"].get<Animation>());
		animController.AddAnimation(animations2["WALKRIGHT"].get<Animation>());
		animController.AddAnimation(animations2["IDLELEFT"].get<Animation>());
		animController.AddAnimation(animations2["IDLERIGHT"].get<Animation>());


		animController.SetActiveAnim(0);

		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		//ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		//ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 40, true, &animController);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 38.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-370.f), float32(20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, OBJECTS | ENVIRONMENT | PLAYER | GROUND, 0.5f, 3.f);


		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetGravityScale(2.5f);
		tempPhsBody.SetFixedRotation(true);
		ECS::GetComponent<CanDamage>(entity).InitBody(tempPhsBody, &animController);
		//tempSpr.SetTransparency(0);
		//add enemy to enemy array
		enemies.push_back(entity);

	}
	*/

	//SetUp Invisible Wall at the left
	EnviroMaker(4500, 10, 200.f, -717.f, 90, 0);

	//SetUp Invisible Wall at the right
	EnviroMaker(4500, 10, 430.f, -717.f, 90, 0);

	//First platform
	BoxMaker(500, 5, 450, -717, 0, 0, 7);

	//First Jump platform
	BoxMaker(40, 5, 405, -687, 0, 0, 7);

	//Angle Platform after jump
	BoxMaker(55, 3, 320, -657, 140, 0, 0.8);

	//First floor platform
	BoxMaker(75, 5, 235, -615, 0, 0, 7);

	//Setup first invis platform
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(218.f), float32(-576.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis1 = entity;
	}

	//Jump platform after invisible platform
	BoxMaker(30, 4, 260, -542, 0, 0, 7);

	//Angled platform
	BoxMaker(45, 4, 305, -512, 45, 0, 0.8);

	//Next Angled platform
	BoxMaker(45, 4, 263, -472, 145, 0, 0.8);

	//Second Floor platform
	BoxMaker(150, 8, 380, -446, 0, 0, 7);

	//Setup second invis 
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(330.f), float32(-406.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis2 = entity;
	}

	//Setup third invis 
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(405.f), float32(-381.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis3 = entity;
	}

	//Setup fourth invis 
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(320.f), float32(-351.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis4 = entity;
	}

	//Setup fifth invis 
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(395.f), float32(-316.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis5 = entity;
	}

	//Setup sixth invis 
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(355.f), float32(-276.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis6 = entity;
	}

	//Platform after invis platforms to reach floor 3
	BoxMaker(45, 9, 295, -246, 0, 0, 7);

	//Setup seventh invis to reach floor 3
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(355.f), float32(-208.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis7 = entity;
	}

	//Floor 3
	BoxMaker(70, 9, 240, -200, 0, 0, 7);

	//Swing stand one
	/*{
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "iSprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 9);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(280), float32(-150));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		tempDef.angle = Transform::ToRadians(0);

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 7, 1);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		auto entity2 = ECS::CreateEntity();
		swings.push_back(entity2);

		//Add components
		ECS::AttachComponent<Sprite>(entity2);
		ECS::AttachComponent<Transform>(entity2);
		ECS::AttachComponent<PhysicsBody>(entity2);
		ECS::AttachComponent<Swing>(entity2);

		//Sets up the components
		std::string fileName2 = "rope.png";

		ECS::GetComponent<Sprite>(entity2).LoadSprite(fileName2, 5, 50);
		ECS::GetComponent<Sprite>(entity2).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity2).SetPosition(vec3(0.f, 30.f, 3.f));


		auto& tempSpr2 = ECS::GetComponent<Sprite>(entity2);
		auto& tempPhsBody2 = ECS::GetComponent<PhysicsBody>(entity2);

		b2Body* tempBody2;
		b2BodyDef tempDef2;
		tempDef2.type = b2_dynamicBody;
		tempDef2.position.Set(float32(280.f), float32(-150.f));

		tempBody2 = m_physicsWorld->CreateBody(&tempDef2);

		tempPhsBody2 = PhysicsBody(entity2, tempBody2, float(tempSpr2.GetWidth() - shrinkX), float(tempSpr2.GetHeight() - shrinkY), vec2(0.f, 0.f), false, HEXAGON, PLAYER | GROUND, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody2.SetRotationAngleDeg(0.f);
		tempPhsBody2.SetFixedRotation(false);
		tempPhsBody2.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody2.SetGravityScale(1.f);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = tempBody;
		jointDef.bodyB = tempBody2;
		jointDef.collideConnected = true;

		jointDef.localAnchorA.Set(0, 0);
		jointDef.localAnchorB.Set(0, tempSpr2.GetHeight() / 2);
		jointDef.enableLimit = true;
		jointDef.lowerAngle = Transform::ToRadians(-45);
		jointDef.upperAngle = Transform::ToRadians(45);

		m_physicsWorld->CreateJoint(&jointDef);
		b2RevoluteJoint* m_joint2;

		m_joint2 = (b2RevoluteJoint*)m_physicsWorld->CreateJoint(&jointDef);
	}*/

	//Swing stand two 
	{
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "iSprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 9);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(305), float32(-105));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		tempDef.angle = Transform::ToRadians(0);

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 7, 1);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		auto entity2 = ECS::CreateEntity();
		swings.push_back(entity2);

		//Add components
		ECS::AttachComponent<Sprite>(entity2);
		ECS::AttachComponent<Transform>(entity2);
		ECS::AttachComponent<PhysicsBody>(entity2);
		ECS::AttachComponent<Swing>(entity2);

		//Sets up the components
		std::string fileName2 = "rope.png";

		ECS::GetComponent<Sprite>(entity2).LoadSprite(fileName2, 5, 50);
		ECS::GetComponent<Sprite>(entity2).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity2).SetPosition(vec3(0.f, 30.f, 3.f));


		auto& tempSpr2 = ECS::GetComponent<Sprite>(entity2);
		auto& tempPhsBody2 = ECS::GetComponent<PhysicsBody>(entity2);

		b2Body* tempBody2;
		b2BodyDef tempDef2;
		tempDef2.type = b2_dynamicBody;
		tempDef2.position.Set(float32(305.f), float32(-105.f));

		tempBody2 = m_physicsWorld->CreateBody(&tempDef2);

		tempPhsBody2 = PhysicsBody(entity2, tempBody2, float(tempSpr2.GetWidth() - shrinkX), float(tempSpr2.GetHeight() - shrinkY), vec2(0.f, 0.f), false, HEXAGON, PLAYER | GROUND, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody2.SetRotationAngleDeg(0.f);
		tempPhsBody2.SetFixedRotation(false);
		tempPhsBody2.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody2.SetGravityScale(1.f);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = tempBody;
		jointDef.bodyB = tempBody2;
		jointDef.collideConnected = true;

		jointDef.localAnchorA.Set(0, 0);
		jointDef.localAnchorB.Set(0, tempSpr2.GetHeight() / 2);
		jointDef.enableLimit = true;
		jointDef.lowerAngle = Transform::ToRadians(-70);
		jointDef.upperAngle = Transform::ToRadians(70);
		
		m_physicsWorld->CreateJoint(&jointDef);
		b2RevoluteJoint* m_joint2;

		m_joint2 = (b2RevoluteJoint*)m_physicsWorld->CreateJoint(&jointDef);
	}

	//Swing stand the
	/*{
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "iSprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 9);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(348), float32(-95));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1);
		tempDef.angle = Transform::ToRadians(0);

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 7, 1);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		auto entity2 = ECS::CreateEntity();
		swings.push_back(entity2);

		//Add components
		ECS::AttachComponent<Sprite>(entity2);
		ECS::AttachComponent<Transform>(entity2);
		ECS::AttachComponent<PhysicsBody>(entity2);
		ECS::AttachComponent<Swing>(entity2);

		//Sets up the components
		std::string fileName2 = "rope.png";

		ECS::GetComponent<Sprite>(entity2).LoadSprite(fileName2, 5, 50);
		ECS::GetComponent<Sprite>(entity2).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity2).SetPosition(vec3(0.f, 30.f, 3.f));


		auto& tempSpr2 = ECS::GetComponent<Sprite>(entity2);
		auto& tempPhsBody2 = ECS::GetComponent<PhysicsBody>(entity2);

		b2Body* tempBody2;
		b2BodyDef tempDef2;
		tempDef2.type = b2_dynamicBody;
		tempDef2.position.Set(float32(348.f), float32(-95.f));

		tempBody2 = m_physicsWorld->CreateBody(&tempDef2);

		tempPhsBody2 = PhysicsBody(entity2, tempBody2, float(tempSpr2.GetWidth() - shrinkX), float(tempSpr2.GetHeight() - shrinkY), vec2(0.f, 0.f), false, HEXAGON, PLAYER | GROUND, 0.5f, 3.f);
			//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody2.SetRotationAngleDeg(0.f);
		tempPhsBody2.SetFixedRotation(false);
		tempPhsBody2.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody2.SetGravityScale(1.f);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = tempBody;
		jointDef.bodyB = tempBody2;
		jointDef.collideConnected = true;

		jointDef.localAnchorA.Set(0, 0);
		jointDef.localAnchorB.Set(0, tempSpr2.GetHeight()/2);
		jointDef.enableLimit = true;
		jointDef.lowerAngle = Transform::ToRadians(-45);
		jointDef.upperAngle = Transform::ToRadians(45);

		m_physicsWorld->CreateJoint(&jointDef);
		b2RevoluteJoint* m_joint2;

		m_joint2 = (b2RevoluteJoint*)m_physicsWorld->CreateJoint(&jointDef);
	}*/
	
	
//Floor 4
BoxMaker(70, 9, 410, -123, 0, 0, 7);

//Setup first invis platform
{
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	std::string fileName = "iSprite.png";
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 3);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(413.f), float32(-75.f));


	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	ECS::AttachComponent<Invisibility>(entity);
	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	invis13 = entity;
}
//Platform 1 to Floor 5
BoxMaker(43, 9, 336, -75, 0, 0, 7);

//Platform 2 to Floor 5
BoxMaker(43, 9, 283, -55, 0, 0, 7);

//Floor 5
BoxMaker(60, 9, 215, -45, 0, 0, 7);

//Platform 1 to Floor 6
BoxMaker(35, 9, 212, -13, 0, 0, 7);

//Platform 2 to Floor 6
BoxMaker(20, 9, 260, 20, 0, 0, 7);

//Platform 3 to Floor 6
BoxMaker(50, 9, 310, 50, 0, 0, 7);

//Platform 4 to Floor 6
BoxMaker(60, 9, 378, 67, 0, 0, 7);

//Platform 5 to Floor 6
BoxMaker(35, 9, 295, 95, 0, 0, 7);

//Setup seventh invis platform 6 to Floor 6
{
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	std::string fileName = "iSprite.png";
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 5);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(230.f), float32(140.f));
	tempDef.angle = Transform::ToRadians(0);


	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	ECS::AttachComponent<Invisibility>(entity);
	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	invis8 = entity;
}

//Platform 7 to Floor 6
BoxMaker(30, 9, 263, 175, 0, 0, 7);

//Platform 8 to Floor 6
BoxMaker(17, 9, 320, 195, 0, 0, 7);

//Setup eighth invis platform 9 to Floor 6
{
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	std::string fileName = "iSprite.png";
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 5);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(350.f), float32(230.f));
	tempDef.angle = Transform::ToRadians(0);


	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	ECS::AttachComponent<Invisibility>(entity);
	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	invis9 = entity;
}

//Floor 6
BoxMaker(80, 9, 233, 245, 0, 0, 7);

//Angle Platform to floor 7
BoxMaker(80, 3, 325, 305, 40, 0, 0.8);

//Floor 7
BoxMaker(60, 9, 410, 355, 0, 0,7);

//Platform 1 to Last Floor
BoxMaker(37, 9, 325, 385, 0, 0, 7);

//Platform 2 to Last Floor
BoxMaker(37, 9, 255, 419, 0, 0, 7);

//Setup ninth invis platform 3 to Last Floor
{
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	std::string fileName = "iSprite.png";
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 37, 5);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(225.f), float32(444.f));
	tempDef.angle = Transform::ToRadians(0);


	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	ECS::AttachComponent<Invisibility>(entity);
	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	invis10 = entity;
}

//Platform 4 to Last Floor
BoxMaker(37, 9, 317, 464, 0, 0, 7);

//Invis Platform 10, Platform 5 to last floor
{
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	std::string fileName = "iSprite.png";
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 37, 5);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(340.f), float32(502.f));
	tempDef.angle = Transform::ToRadians(0);


	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	ECS::AttachComponent<Invisibility>(entity);
	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	invis11 = entity;
}

//Platform 6 to last floor
BoxMaker(30, 9, 245, 533, 0, 0, 7);


//Invis Platform 11, Platform 7 to last floor
/*{
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	std::string fileName = "iSprite.png";
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 5);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(295.f), float32(560.f));
	tempDef.angle = Transform::ToRadians(0);


	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.2f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	ECS::AttachComponent<Invisibility>(entity);
	ECS::GetComponent<Invisibility>(entity).set_entity(entity);
	invis12 = entity;
}*/
//Platform 8 to Last Floor
BoxMaker(45, 9, 347, 559, 0, 0, 7);

//Platform 9 to Last Floor
BoxMaker(27, 7, 298, 598, 0, 0, 7);

//Last floor part 1
BoxMaker(80, 9, 400, 623, 0, 0, 7);

//Last Floor Part2
BoxMaker(83, 9, 230, 623, 0, 0, 7);


	{//Health bar (green)

		healthBar = Scene::createHealthBar();
	}

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
		tempDef.position.Set(float32(250.f), float32(-710.f)); //Starting position 250 x, -710 y

		tempBody = m_physicsWorld->CreateBody(&tempDef);


		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER |ENVIRONMENT, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -4.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | PTRIGGER | GROUND, 0.4f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT|ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		ECS::GetComponent<AnimationController>(entity).SetActiveAnim(1); //right
		tempPhsBody.SetGravityScale(2.5f);
	}
	//ECS::GetComponent<Player_Power>(p_entity).m_power[1] = true;
	MainEntities::Powerups(std::vector<bool> {false, true});
}

void LevelTwo::Update()
{
	//Fmod.Update();

	auto& player2 = ECS::GetComponent<Player>(p_entity);

	//setup animation component again so the player doesnt lose their animations
	player2.ReassignComponents(&ECS::GetComponent<AnimationController>(p_entity), &ECS::GetComponent<Sprite>(p_entity));
	player2.Update();
	auto& animations = ECS::GetComponent<AnimationController>(p_entity);

	for (int i = 0; i < enemies.size(); i++)
	{
		if (alive[i])
		{
			auto& enemy_c = ECS::GetComponent<CanDamage>(enemies[i]);
			enemy_c.Walk();
			
			if (enemy_c.moving)
			{
				ECS::GetComponent<AnimationController>(enemies[i]).SetActiveAnim(0 + enemy_c.facing);
			}
			else //idle
			{
				ECS::GetComponent<AnimationController>(enemies[i]).SetActiveAnim(0 + 2 + enemy_c.facing);
			}
			if (enemy_c.attack)
			{

			}
			
			//if (ECS::GetComponent<PhysicsBody>(enemies[i]).GetPosition().y <= 0)
			{
				//enemy_c.hp = 0;
			}
			//check if enemy is dead
			if (enemy_c.hp <= 0)
			{
				
				enemy_c.m_candamage = false;
				ECS::DestroyEntity(enemies[i]);
				//PhysicsBody::m_bodiesToDelete.push_back(enemies[i]);

				alive[i] = false;
				//player2.ReassignComponents(&ECS::GetComponent<AnimationController>(p_entity), &ECS::GetComponent<Sprite>(p_entity));
				//player2.Update();

			}
		}
	}


	ECS::GetComponent<Invisibility>(invis1).update_invisible();
	ECS::GetComponent<Invisibility>(invis2).update_invisible();
	ECS::GetComponent<Invisibility>(invis3).update_invisible();
	ECS::GetComponent<Invisibility>(invis4).update_invisible();
	ECS::GetComponent<Invisibility>(invis5).update_invisible();
	ECS::GetComponent<Invisibility>(invis6).update_invisible();
	ECS::GetComponent<Invisibility>(invis7).update_invisible();
	ECS::GetComponent<Invisibility>(invis8).update_invisible();
	ECS::GetComponent<Invisibility>(invis9).update_invisible();
	ECS::GetComponent<Invisibility>(invis10).update_invisible();
	ECS::GetComponent<Invisibility>(invis11).update_invisible();
	//ECS::GetComponent<Invisibility>(invis12).update_invisible();
	ECS::GetComponent<Invisibility>(invis13).update_invisible();

	auto& dash = ECS::GetComponent<CanJump>(p_entity);


	if (dash.hp <= 0) //dying
	{
		is_done = false;
		alive.clear();
		swings.clear();
		enemies.clear();
		inputS.open("Progress.txt");
		if (inputS.is_open())
		{
			inputS << 3 << "\n";
		}
		inputS.close();
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
			player.SetGravityScale(2.3);
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
			player.SetGravityScale(2.1);
		}
		else
		{
			player.SetGravityScale(1.5);
		}
	}


	hb.UpdateHealthBar(healthBar);
	hb.UpdatePowers(pcount);

	//end check
	if (player.GetPosition().x >= 410 && player.GetPosition().y >= 625)
	{
		selection = 8; //next scene
		Sound.Mute();
		is_done = true;
		inputS.open("HP.txt");
		if (inputS.is_open())
		{
			inputS << dash.hp << "\n";
		}
		inputS.close();
		inputS.open("Progress.txt");
		if (inputS.is_open())
		{
			inputS << 4 << "\n";
		}
		inputS.close();
	}
	if (attached)
	{
		//ECS::GetComponent<PhysicsBody>(rope1).SetVelocity(player.GetVelocity());
		for (int i = 0; i < swings.size(); i++)
		{
			if (ECS::GetComponent<Swing>(swings[i]).is_swing)
			{
				float angle = 45;
				auto& sb = ECS::GetComponent<PhysicsBody>(swings[i]);
				if (player.GetPosition().x > sb.GetPosition().x + 20 || player.GetPosition().x < sb.GetPosition().x - 20)
				{
					angle = 60;
				}
				if (player.GetPosition().x < sb.GetPosition().x)
				{
					angle *= -1;
				}
				if (player.GetPosition().x <= sb.GetPosition().x + 15 && player.GetPosition().x >= sb.GetPosition().x - 15)
				{
					angle = 0;
				}

				sb.SetRotationAngleDeg(angle);
			}
		}
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));


}

void LevelTwo::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(p_entity);

	b2Vec2 vel = b2Vec2(0.f, 0.f);
	auto& canJump = ECS::GetComponent<CanJump>(p_entity);

	b2Vec2 pvel = player.GetBody()->GetLinearVelocity();

	if (Input::GetKey(Key::A) || Input::GetKey(Key::LeftArrow)) //left
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
		if (attached)
		{
			//player.GetBody()->SetLinearVelocity(b2Vec2(-4000.f * speed, 400.f));
			////dot product
			//float angle = m_joint->GetAnchorA().x * m_joint->GetAnchorB().x + m_joint->GetAnchorA().y * m_joint->GetAnchorB().y;
			//player.GetPosition().Normalize();
			//auto mult =player.GetMass() * -9.8 * cos(angle) + player.GetMass() * player.GetBody()->GetLinearVelocity().LengthSquared() / m_joint->GetMaxLength();
			//player.GetBody()->ApplyForceToCenter(b2Vec2(player.GetPosition().x*mult*100, player.GetPosition().y*mult*100), true);

			//player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 400000.f), true);
			//player.GetBody()->SetLinearVelocity(b2Vec2(-400000.f * speed, 0.f));
		}


	}
	else if (Input::GetKey(Key::D) || Input::GetKey(Key::RightArrow)) //right
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
		if (attached)
		{
			//player.GetBody()->SetLinearVelocity(b2Vec2(4000.f * speed, 400.f));
			//player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 400000.f), true);
			//player.GetBody()->SetLinearVelocity(b2Vec2(400000.f * speed, 0.f));
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

void LevelTwo::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(p_entity);
	auto& sprite = ECS::GetComponent<Sprite>(p_entity);

	auto& canJump = ECS::GetComponent<CanJump>(p_entity);
	auto& power = ECS::GetComponent<Player_Power>(p_entity);

	auto& anims = ECS::GetComponent<Player>(p_entity);

	auto& vel = player.GetBody()->GetLinearVelocity();
	auto& pos = player.GetBody()->GetPosition();
	//auto& dash = ECS::GetComponent<CanJump>(p_entity);

	if (Input::GetKey(Key::E))
	{
		if (attached)
		{
			m_physicsWorld->DestroyJoint(m_joint);
			attached = false;
			canJump.can_swing = false;
			player.GetBody()->SetLinearVelocity(b2Vec2(vel.x, 1600000000));
			for (int i = 0; i < swings.size(); i++)
			{
				ECS::GetComponent<Swing>(swings[i]).is_swing = false;
				ECS::GetComponent<PhysicsBody>(swings[i]).SetRotationAngleDeg(0);
			}
		}
		if (canJump.can_swing && !attached && !power.m_power[1] && !power.m_power[0])
		{
			for (int i = 0; i < swings.size(); i++)
			{
				if (ECS::GetComponent<Swing>(swings[i]).m_swing)
				{
					b2RopeJointDef jointDef;
					jointDef.bodyB = player.GetBody();
					jointDef.bodyA = ECS::GetComponent<PhysicsBody>(swings[i]).GetBody();
					//jointDef.collideConnected = true;

					jointDef.localAnchorB.Set(0, player.GetHeight() / 2);
					jointDef.localAnchorA.Set(0, ECS::GetComponent<Sprite>(swings[i]).GetHeight() / 2);
					jointDef.maxLength = ECS::GetComponent<Sprite>(swings[i]).GetHeight();
					//jointDef.length = ECS::GetComponent<Sprite>(swings[i]).GetHeight();
					//jointDef.maxLength = 1;

					//m_physicsWorld->CreateJoint(&jointDef);


					m_joint = (b2RopeJoint*)m_physicsWorld->CreateJoint(&jointDef);

					attached = true;
					ECS::GetComponent<Swing>(swings[i]).is_swing = true;
				}
			}
		}
		
	}

	if (Input::GetKey(Key::K))
	{
		if (canJump.m_canJump)
		{
			if (!power.m_power[1] && !power.m_power[0])
			{//manual box collision calculation
				anims.m_attack = true;
				anims.m_locked = true;
				anims.m_moving = false;
				for (int i = 0; i < enemies.size(); i++)
				{
					if (alive[i])
					{
						Scene::Attack(p_entity, enemies[i]);

					}
				}
			}
		}
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	//if (Input::GetKeyDown(Key::One)) //jump higher ability
	//{
	//	if (MainEntities::Powerups()[0])
	//	{
	//		power.m_power[0] = !power.m_power[0]; //reverses choice
	//	}
	//}

	if (Input::GetKeyDown(Key::Two)) //vision
	{
		if (!anims.m_attack)
		{
			if (MainEntities::Powerups()[1])
			{
				power.m_power[1] = !power.m_power[1];
				power.m_power[0] = !power.m_power[0];//reverses choice
				//manually change values
				ECS::GetComponent<Invisibility>(invis1).is_invisible = !ECS::GetComponent<Invisibility>(invis1).is_invisible;
				ECS::GetComponent<Invisibility>(invis2).is_invisible = !ECS::GetComponent<Invisibility>(invis2).is_invisible;
				ECS::GetComponent<Invisibility>(invis3).is_invisible = !ECS::GetComponent<Invisibility>(invis3).is_invisible;
				ECS::GetComponent<Invisibility>(invis4).is_invisible = !ECS::GetComponent<Invisibility>(invis4).is_invisible;
				ECS::GetComponent<Invisibility>(invis5).is_invisible = !ECS::GetComponent<Invisibility>(invis5).is_invisible;
				ECS::GetComponent<Invisibility>(invis6).is_invisible = !ECS::GetComponent<Invisibility>(invis6).is_invisible;
				ECS::GetComponent<Invisibility>(invis7).is_invisible = !ECS::GetComponent<Invisibility>(invis7).is_invisible;
				ECS::GetComponent<Invisibility>(invis8).is_invisible = !ECS::GetComponent<Invisibility>(invis8).is_invisible;
				ECS::GetComponent<Invisibility>(invis9).is_invisible = !ECS::GetComponent<Invisibility>(invis9).is_invisible;
				ECS::GetComponent<Invisibility>(invis10).is_invisible = !ECS::GetComponent<Invisibility>(invis10).is_invisible;
				ECS::GetComponent<Invisibility>(invis11).is_invisible = !ECS::GetComponent<Invisibility>(invis11).is_invisible;
				//ECS::GetComponent<Invisibility>(invis12).is_invisible = !ECS::GetComponent<Invisibility>(invis12).is_invisible;
				ECS::GetComponent<Invisibility>(invis13).is_invisible = !ECS::GetComponent<Invisibility>(invis13).is_invisible;
				if (power.m_power[1]) {
					player.SetCenterOffset(vec2(0, 0));
					b2Body* playerBody = m_sceneReg->get<PhysicsBody>(p_entity).GetBody();
					b2PolygonShape tempBox;
					tempBox.SetAsBox(player.GetWidth() * 1.5 / 2, player.GetHeight() / 2 / 1.2);

					b2FixtureDef wolfBox;
					wolfBox.shape = &tempBox;
					wolfBox.density = 2.5f;
					wolfBox.friction = 1.2f;
					wolfBox.filter.categoryBits = playerBody->GetFixtureList()->GetFilterData().categoryBits;
					wolfBox.filter.maskBits = playerBody->GetFixtureList()->GetFilterData().maskBits;


					playerBody->DestroyFixture(playerBody->GetFixtureList());
					playerBody->CreateFixture(&wolfBox);
					playerBody->SetGravityScale(5.0f);
					player.GetBody()->SetTransform(player.GetPosition() + b2Vec2(0, 0), Transform::ToRadians(90));
					//player.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

				}
				else
				{
					player.GetBody()->SetTransform(player.GetPosition() + b2Vec2(0, 10), 0);
					b2Body* playerBody = m_sceneReg->get<PhysicsBody>(p_entity).GetBody();
					b2PolygonShape tempBox;
					tempBox.SetAsBox(player.GetWidth() / 2, player.GetHeight() / 2, b2Vec2(0, -4), 0);

					b2FixtureDef normalBox;
					normalBox.shape = &tempBox;
					normalBox.density = 2.0f;
					normalBox.friction = 2.0f;
					//normalBox.filter.categoryBits = playerBody->GetFixtureList()->GetFilterData().categoryBits;
					normalBox.filter.categoryBits = PLAYER;
					normalBox.filter.maskBits = playerBody->GetFixtureList()->GetFilterData().maskBits;

					playerBody->DestroyFixture(playerBody->GetFixtureList());
					playerBody->SetGravityScale(5.0f);
					playerBody->CreateFixture(&normalBox);
					player.SetCenterOffset(vec2(0.f, -4.f));
					//player.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				}
				jump_high = !jump_high;

				//ECS::GetComponent<Invisibility>(invis8).is_invisible = !ECS::GetComponent<Invisibility>(invis8).is_invisible;
				//ECS::GetComponent<Invisibility>(invis9).is_invisible = !ECS::GetComponent<Invisibility>(invis9).is_invisible;
			}
		}
	}

	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			if (power.m_power[0]) //jump higher
			{
				player.SetGravityScale(1.7f);
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
		if (!power.m_power[1] && !power.m_power[0])
		{
			auto& a = ECS::GetComponent<AnimationController>(p_entity);
			//reset to allow dash animation
			a.GetAnimation(a.GetActiveAnim()).Reset();
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
				anims.m_dash = true;
				anims.m_locked = true;
				anims.m_moving = false;

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

			//dash cooldown
			if (!canJump.can_dash && dashcooldown)
			{
				dashtime = clock();
				dashcooldown = false;
			}
		}
	}

}

void LevelTwo::KeyboardUp()
{


}
