#include "TitleScreen.h"
#include "Utilities.h"

int changeCount = 0;
int selectionCounter = 1;
int selector = 0;
int background = 0;
int selection = -1;
TitleScreen::TitleScreen(std::string name)
{
	m_gravity = b2Vec2(0.f, -98.f);

	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);
}

void TitleScreen::InitScene(float windowWidth, float windowHeight)
{

	selectionCounter = 1;
	selection = -1;
	changeCount = 0;
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 3.f));

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
		std::string fileName = "TITLESCRN.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 220, 110);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 1.f));
		background = entity;
	}



	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	setSelect(1);
}

void TitleScreen::Update()
{
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
		if (selectionCounter - 1 > 0) {
			selectionCounter--;
		}
		else
		{
			selectionCounter = 3;
		}
		setSelect(selectionCounter);
	}
	if (Input::GetKeyDown(Key::DownArrow) || Input::GetKeyDown(Key::S))
	{
		if (selectionCounter + 1 < 4) {
			selectionCounter++;
		}
		else
		{
			selectionCounter = 1;
		}
		setSelect(selectionCounter);

	}

	if (Input::GetKeyDown(Key::Space) || Input::GetKeyDown(Key::Enter))
	{
		if (selectionCounter == 1) {
			selection = 0;
		}
		if (selectionCounter == 2) {
			selection = 15;
		}
		if (selectionCounter == 3) {
			exit(0);
		}
	}
}

void TitleScreen::KeyboardUp()
{
}
void TitleScreen::setSelect(int i) {
	auto& selectTransform = ECS::GetComponent<Transform>(selector);
	auto& selectSprite = ECS::GetComponent<Sprite>(selector);
	selectTransform.SetPositionX(setX(i));
	selectTransform.SetPositionY(setY(i));
	selectSprite.SetWidth(setW(i));
	selectSprite.SetHeight(setH(i));

}

int TitleScreen::setX(int i) {
	return 2;
}
int TitleScreen::setY(int i) {
	switch (i) {
	case 1:
		return 15;
		break;
	case 2:
		return 0;
		break;
	case 3:
		return -16;
		break;
	}
}
int TitleScreen::setW(int i) {
	switch (i) {
	case 1:
		return 50;
		break;
	case 2:
		return 40;
		break;
	case 3:
		return 40;
		break;
	}
}
int TitleScreen::setH(int i) {
	return 15;
}

