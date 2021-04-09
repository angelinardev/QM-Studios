#include "Credits2.h"
#include "Utilities.h"


Credits2::Credits2(std::string name)
{
	m_gravity = b2Vec2(0.f, -98.f);
}

void Credits2::InitScene(float windowWidth, float windowHeight)
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

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(background));
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset(200);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(0);

	is_done = false;
	
}

void Credits2::Update()
{
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).Update();
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).Update();

}

int Credits2::ChangeScene()
{

	return selection;

}

void Credits2::InitTexture()
{
	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetGravity(m_gravity);

	m_sceneReg = new entt::registry;
	//Attach the register
	ECS::AttachRegister(m_sceneReg);
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		background = entity;
		//Add components  
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "credsfin.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 215, 120);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));


	}

}

void Credits2::KeyboardHold()
{
}

void Credits2::KeyboardDown()
{

	if (Input::GetKeyDown(Key::Space) || Input::GetKeyDown(Key::Enter))
	{
		
		exit(0);
	}
}

void Credits2::KeyboardUp()
{
}


