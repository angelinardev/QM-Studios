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
void LevelOne::InitTexture()
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
		std::string fileName = "lvl1back.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1900, 300);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(315.f, 0.f, 2.f));
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
		std::string fileName = "lvl1filler.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1900, 300);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(360.f, 40.f, 1.f));
	}

	//enemy
	
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
	

	//SetUp Invisible Wall at the beginning
	EnviroMaker(1000, 20, -550.f, 0.f, 90, 0);

	//Setup spawning static Platform
	BoxMaker(210, 5, -440.f, -15.f, 0, 0, 6);

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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 5);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-300.f), float32(10.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 7.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis1 = entity;
	}

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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 5);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-250.f), float32(40.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 7.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis2 = entity;
	}
	////2nd enemy
	//{

	//	auto entity = ECS::CreateEntity();
	//	//Add components  

	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);
	//	ECS::AttachComponent<AnimationController>(entity);
	//	ECS::AttachComponent<CanDamage>(entity);

	//	ECS::GetComponent<CanDamage>(entity).m_candamage = true;

	//	//Sets up the components  

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& animController = ECS::GetComponent<AnimationController>(entity);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));
	//	//Sets up the components  
	//	std::string fileName = "spritesheets/neville.png";
	//	std::string animations = "Boss1.json";

	//	animController.InitUVs(fileName);
	//	nlohmann::json animations2 = File::LoadJSON(animations);
	//	animController.AddAnimation(animations2["Boss1Idle"].get<Animation>());
	//	animController.AddAnimation(animations2["Boss1Fingermove"].get<Animation>());
	//	animController.AddAnimation(animations2["Boss1Yawn"].get<Animation>());
	//	animController.AddAnimation(animations2["Boss1Suck"].get<Animation>());
	//	animController.AddAnimation(animations2["Boss1Suck2"].get<Animation>());//face right?
	//	animController.SetActiveAnim(0);

	//	//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
	//	//ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	//	//ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));

	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30, true, &animController);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_dynamicBody;
	//	tempDef.position.Set(float32(-250.f), float32(45.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, OBJECTS | ENVIRONMENT | PLAYER | GROUND, 0.5f, 3.f);


	//	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	//	tempPhsBody.SetGravityScale(2.5f);
	//	tempPhsBody.SetFixedRotation(true);
	//	ECS::GetComponent<CanDamage>(entity).InitBody(tempPhsBody, animController);
	//	//tempSpr.SetTransparency(0);
	//	//add enemy to enemy array
	//	enemies.push_back(entity);

	//}

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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 5);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-200.f), float32(30.f));


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 7.2f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis3 = entity;
	}

	//Setup fourth invis platform
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		std::string fileName = "iSprite.png";
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 5.f));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 55, 5);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-153.f), float32(-5.f));
		tempDef.angle = Transform::ToRadians(125);


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis4 = entity;
	}

	//testing pickup
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		///Sets up the components  
		std::string fileName = "spritesheets/paper.png";
		std::string animations = "Paper.json";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		nlohmann::json animations2 = File::LoadJSON(animations);
		animController.AddAnimation(animations2["Paper"].get<Animation>());
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 20, true, &animController);
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PickupTrigger(2); //first powerup
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-375), float32(0));
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	//Slide to the next static platform
	BoxMaker(40, 5, -130, -40, 125, 0);

	//Static Platform after slide
	BoxMaker(60, 5, -80, -60, 0, 0,6);

	//Rock
	EnviroMaker(5, 5, -82, -50, 90, 0);
	BoxMaker(30, 5, -65, -43.5, 10, 0);
	BoxMaker(25, 5, -43, -50, 135,0,0.8);

	//Static platform after rock
	BoxMaker(245, 5, 80, -60, 0, 0,6);
	//3rd enemy
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
		tempDef.position.Set(float32(80.f), float32(-40.f));

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
	//spawn heart
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		//Sets up the components  
		std::string fileName = "spritesheets/health.png";
		std::string animations = "Health.json";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		nlohmann::json animations2 = File::LoadJSON(animations);
		animController.AddAnimation(animations2["Health"].get<Animation>());
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new HealthTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(130), float32(-50));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, PTRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	//Static platform after jump // This is where they should die 
	BoxMaker(235, 5, 410, -60, 0, 0,6);
	//4th enemy
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
		tempDef.position.Set(float32(410.f), float32(-40.f));

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

	//Upwards platform
	BoxMaker(100, 5, 558, -50,20,0, 0.8);

	//Static platform after upwards
	BoxMaker(95, 5, 650, -35, 0, 0, 2);

	//Platform Jump 1
	BoxMaker(45, 2, 614, -10, 0, 0, 2);
	//Platform Jump 2
	BoxMaker(45, 5, 669, 25, 0, 0, 2);
	//Platform Jump 3
	BoxMaker(25, 5, 590, 45, 0, 0, 2);
	//Wall
	EnviroMaker(105, 5, 690, 10, 90, 0);


	//Downwards platform after Platfrom Jump 3
	BoxMaker(75, 5, 725, 49, 160, 0);

	//Downwards platform heading towards to rock
	BoxMaker(80, 5, 788, 10, 135, 0);
	//5th enemy
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
		tempDef.position.Set(float32(788.f), float32(10.f));

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

	//Second Rock
	BoxMaker(28, 5, 835, -13, 0, 0);
	BoxMaker(28, 5, 860, -22, 0, 0);

	//Static platform
	BoxMaker(235, 5, 945, -50, 0, 0, 6);

	//Wall 
	EnviroMaker(100, 5, 1060, -10, 90, 0, 6);


	//Setup fifth invis platform
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
		tempDef.position.Set(float32(970.f), float32(-5.f));
		tempDef.angle = Transform::ToRadians(45);


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 0.4f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis5 = entity;
	}

	//Setup six invis platform
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
		tempDef.position.Set(float32(900.f), float32(10.f));
		tempDef.angle = Transform::ToRadians(135);


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 0.4f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis6 = entity;
	}

	//Setup seventh invis platform
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
		tempDef.position.Set(float32(970.f), float32(45.f));
		tempDef.angle = Transform::ToRadians(40);


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 0.4f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::AttachComponent<Invisibility>(entity);
		ECS::GetComponent<Invisibility>(entity).set_entity(entity);
		invis7 = entity;
	}


	//downwards platform
	BoxMaker(25, 5, 1055, 48, 165, 0, 0.8);
	
	//Final Static platform 
	BoxMaker(200, 5, 1150, 45, 0, 0, 6);

	//Invisible Wall at the end
	EnviroMaker(350, 5, 1200, 50, 90, 0, 6);

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
		tempDef.position.Set(float32(-530.f), float32(0.f)); //Starting position -530 x, 0 y

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
				ECS::GetComponent<PhysicsBody>(enemies[i]).DeleteBody();
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
	//ECS::GetComponent<Invisibility>(invis8).update_invisible();
	//ECS::GetComponent<Invisibility>(invis9).update_invisible();

	auto& dash = ECS::GetComponent<CanJump>(p_entity);
	

	if (dash.hp <= 0) //dying
	{
		//play death animation
		player2.m_locked = true;
		animations.SetActiveAnim(DEATH + player2.m_facing);
		if (animations.GetAnimation(animations.GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			//player2.m_locked = false;
			
			//Resets the attack animation
			//animations.GetAnimation(animations.GetActiveAnim()).Reset();
			is_done = false;
			alive.clear();
			enemies.clear();
			inputS.open("Progress.txt");
			if (inputS.is_open())
			{
				inputS << 2 << "\n";
			}
			inputS.close();
			selection = 2; //end screen? for now
		}
		

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

	//jump pit check
	if (player.GetPosition().y <= -80 && player.GetPosition().x <= 410 && player.GetPosition().x >= 100)
	{
		auto& power = ECS::GetComponent<Player_Power>(p_entity);
		if (!power.m_power[1] && !power.m_power[0])
		{
			player.GetBody()->SetTransform(b2Vec2(170, -40), 0);
		}
		else
		{
			player.GetBody()->SetTransform(b2Vec2(170, -40), Transform::ToRadians(90));
		}
		dash.hp -= 25;
	}

	//first pit check
	if (player.GetPosition().y <= -60 && player.GetPosition().x <= -100)
	{
		auto& power = ECS::GetComponent<Player_Power>(p_entity);
		if (!power.m_power[1] && !power.m_power[0])
		{
			player.GetBody()->SetTransform(b2Vec2(-400, 10), 0);
		}
		else
		{
			player.GetBody()->SetTransform(b2Vec2(-400, 10), Transform::ToRadians(90));
		}
		dash.hp -= 25;
	}

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
			player.SetGravityScale(1.8);
		}
	}


	hb.UpdateHealthBar(healthBar);
	hb.UpdatePowers(pcount);

	//end check
	if (player.GetPosition().x >= 1150 && player.GetPosition().y >= 50)
	{
		selection = 7; //next scene
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
			inputS << 3 << "\n";
		}
		inputS.close();
	}

	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(p_entity));


}

void LevelOne::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(p_entity);

	b2Vec2 vel = b2Vec2(0.f, 0.f);
	auto& canJump = ECS::GetComponent<CanJump>(p_entity);

	b2Vec2 pvel = player.GetBody()->GetLinearVelocity();

	if (Input::GetKey(Key::A)|| Input::GetKey(Key::LeftArrow)) //left
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
	else if (Input::GetKey(Key::D)||Input::GetKey(Key::RightArrow)) //right
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
	auto& sprite = ECS::GetComponent<Sprite>(p_entity);

	auto& canJump = ECS::GetComponent<CanJump>(p_entity);
	auto& power = ECS::GetComponent<Player_Power>(p_entity);

	auto& anims = ECS::GetComponent<Player>(p_entity);

	auto& vel = player.GetBody()->GetLinearVelocity();
	auto& pos = player.GetBody()->GetPosition();
	//auto& dash = ECS::GetComponent<CanJump>(p_entity);

	int spriteHeight;
	int spriteWidth;

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
					DashFX.SetVolume(0);
					DashFX.Play();
					player.GetBody()->SetLinearVelocity(b2Vec2(-1000000, vel.y));
					canJump.can_dash = false;
				}
				else if (facing == 1)
				{
					//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(400000.f * 1000, 0.f), true);
					//player.GetBody()->SetTransform(b2Vec2(pos.x + 30, pos.y), 0);
					DashFX.SetVolume(0);
					DashFX.Play();
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
					DashFX.SetVolume(0);
					DashFX.Play();
					//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-400000.f * 1000, 0.f), true);
					player.GetBody()->SetTransform(b2Vec2(pos.x - 30, pos.y), 0);
					//player.GetBody()->SetLinearVelocity(b2Vec2(-1000000, vel.y));
					canJump.can_dash = false;
				}
				else if (facing == 1)
				{
					DashFX.SetVolume(0);
					DashFX.Play();
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

void LevelOne::KeyboardUp()
{


}
