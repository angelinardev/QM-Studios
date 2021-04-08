#include "CreditScene.h"
#include "Utilities.h"


CreditScene::CreditScene(std::string name)
{
	m_gravity = b2Vec2(0.f, -98.f);
}

void CreditScene::InitScene(float windowWidth, float windowHeight)
{
	selection = -1;
	
	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;


	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-65.f, 65.f, -80, 25.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	int background;
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "background.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 215, 120);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 1.f));
		background = entity;
	}
	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset(200);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(0);


	setSelect(1);
	selectionCounter == 1;
	is_done = false;
	Sound.Play();
}

void CreditScene::Update()
{
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).Update();
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).Update();

	ECS::GetComponent<Transform>(selector).SetPositionX(0);
	ECS::GetComponent<Transform>(selector).SetPositionY(-20);
	ECS::GetComponent<Sprite>(selector).SetWidth(170);
	ECS::GetComponent<Sprite>(selector).SetHeight(20);
}

int CreditScene::ChangeScene()
{

	return selection;

}

void CreditScene::InitTexture()
{
	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_sceneReg = new entt::registry;
	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up the components
		std::string fileName = "Selector.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-100.f, 30.f, 3.f));



		selector = entity;
	}
	//fireflies
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components  
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "creditsfull.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 215, 130);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 25.f, 2.f));


	}

}

void CreditScene::KeyboardHold()
{
}

void CreditScene::KeyboardDown()
{
	
	if (Input::GetKeyDown(Key::Space) || Input::GetKeyDown(Key::Enter))
	{
		selection = 0; //return to menu
		Sound.Mute();
	}
}

void CreditScene::KeyboardUp()
{
}
void CreditScene::setSelect(int i) {
	auto& selectTransform = ECS::GetComponent<Transform>(selector);
	auto& selectSprite = ECS::GetComponent<Sprite>(selector);

	selectTransform.SetPositionX(-40);
	selectTransform.SetPositionY(setY(i));
	selectSprite.SetWidth(setW(i));
	selectSprite.SetHeight(13);

}

int CreditScene::setY(int i) {
	switch (i) {
	case 1:
		return -2;
		break;
	case 2:
		return 12;
		break;
	case 3:
		return 24;
		break;
	case 4:
		return 36;
		break;
	}
}
int CreditScene::setW(int i) {
	switch (i) {
	case 1:
		return 45;
		break;
	case 2:
		return 75;
		break;
	case 3:
		return 53;
		break;
	case 4:
		return 88;
		break;
	}
}


