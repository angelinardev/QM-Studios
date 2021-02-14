#include "TitleScreen.h"
#include "Utilities.h"


TitleScreen::TitleScreen(std::string name)
{
	m_gravity = b2Vec2(0.f, -98.f);

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);
}

void TitleScreen::InitScene(float windowWidth, float windowHeight)
{
	selection = -1;
	
	m_sceneReg = new entt::registry;
	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;


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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40.f, 30.f, 3.f));

		selector = entity;
	}

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
	//fireflies
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		firef = entity;

		//Add components  
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 20.f, 2.f));
		//Sets up the components  
		std::string fileName = "spritesheets/fireflies.png";
		std::string animations = "fireflies.json";

		animController.InitUVs(fileName);
		nlohmann::json animations2 = File::LoadJSON(animations);
		animController.AddAnimation(animations2["FLICKER"].get<Animation>());
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 215, 120, true, &animController);

	}
	//options
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Menu_Text.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 70);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40.f, 30.f, 3.f));
		
	}
	//logo
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "QM_Studios_Transparent.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(88.f, -20.f, 3.f));

	}
	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset(200);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(0);
	

	setSelect(4);
	selectionCounter == 4;
}

void TitleScreen::Update()
{
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).Update();
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).Update();
}

int TitleScreen::ChangeScene()
{

	return selection;

}

void TitleScreen::KeyboardHold()
{
}

void TitleScreen::KeyboardDown()
{
	if (Input::GetKeyDown(Key::UpArrow) || Input::GetKeyDown(Key::W))
	{
		if (selectionCounter + 1 < 5) {
			selectionCounter++;
		}
		else
		{
			selectionCounter = 1;
		}
		setSelect(selectionCounter);
	}
	if (Input::GetKeyDown(Key::DownArrow) || Input::GetKeyDown(Key::S))
	{
		if (selectionCounter - 1 > 0) {
			selectionCounter--;
		}
		else
		{
			selectionCounter = 4;
		}
		setSelect(selectionCounter);

	}

	if (Input::GetKeyDown(Key::Space) || Input::GetKeyDown(Key::Enter))
	{
		if (selectionCounter == 1) {
			exit(0);
		}
		if (selectionCounter == 2) {
			exit(0);
		}
		if (selectionCounter == 3) {
			exit(0);
		}
		if (selectionCounter == 4) {
			
			selection = 1;
		}
	}
}

void TitleScreen::KeyboardUp()
{
}
void TitleScreen::setSelect(int i) {
	auto& selectTransform = ECS::GetComponent<Transform>(selector);
	auto& selectSprite = ECS::GetComponent<Sprite>(selector);

	selectTransform.SetPositionX(-40);
	selectTransform.SetPositionY(setY(i));
	selectSprite.SetWidth(setW(i));
	selectSprite.SetHeight(13);

}

int TitleScreen::setY(int i) {
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
int TitleScreen::setW(int i) {
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


