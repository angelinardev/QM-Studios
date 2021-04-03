#include "EndScreen.h"
#include "Utilities.h"


EndScreen::EndScreen(std::string name)
{
	m_gravity = b2Vec2(0.f, -98.f);

}

void EndScreen::InitScene(float windowWidth, float windowHeight)
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
		std::string fileName = "unknown2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 235, 121);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 20.f, 1.f));
		background = entity;
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset(190);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(0);


	setSelect(1);
	selectionCounter == 1;
}

void EndScreen::InitTexture()
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40.f, 30.f, 3.f));

		selector = entity;
	}

}

void EndScreen::Update()
{
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).Update();
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).Update();
}

int EndScreen::ChangeScene()
{
	return selection;

}

void EndScreen::KeyboardHold()
{
}

void EndScreen::KeyboardDown()
{
	if (Input::GetKeyDown(Key::UpArrow) || Input::GetKeyDown(Key::W))
	{
		if (selectionCounter + 1 < 3) {
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
			selectionCounter = 2;
		}
		setSelect(selectionCounter);

	}

	if (Input::GetKeyDown(Key::Space) || Input::GetKeyDown(Key::Enter))
	{
		if (selectionCounter == 1) {
			exit(0);
		}
		if (selectionCounter == 2) {
			inputS.open("Progress.txt");
			int var =0;
			if (inputS.is_open())
			{
				inputS >> var;
			}
			if (var == 1)
			{
				selection = 1; //last save?
			}
			if (var == 2)
			{
				selection = 4;
			}
			if (var == 3)
			{
				selection = 7;
			}
		}
	}
}

void EndScreen::KeyboardUp()
{
}
void EndScreen::setSelect(int i) {
	auto& selectTransform = ECS::GetComponent<Transform>(selector);
	auto& selectSprite = ECS::GetComponent<Sprite>(selector);

	selectTransform.SetPositionX(2);
	selectTransform.SetPositionY(setY(i));
	selectSprite.SetWidth(setW(i));
	selectSprite.SetHeight(15);

}

int EndScreen::setY(int i) {
	switch (i) {
	case 1:
		return -16;
		break;
	case 2:
		return 10;
		break;
	default:
		return -16;
		
	
	}
}
int EndScreen::setW(int i) {
	switch (i) {
	case 1:
		return 40;
		break;
	case 2:
		return 80;
		break;
	default:
		return 40;
	
	}
}


