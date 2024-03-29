#include "Game.h"

#include <random>

const int FPS = 60;
const int frameDelay = 1000 / FPS;


Uint32 frameStart;
int frameTime;

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Initializes the backend with window width and height values
	BackEnd::InitBackEnd(719.f, 436.f);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new scene.
	//Replace this with your own scene.
	m_scenes.push_back(new TitleScreen("The Beyond"));
	m_scenes.push_back(new PhysicsPlayground("The Beyond"));
	m_scenes.push_back(new EndScreen("The Beyond"));
	m_scenes.push_back(new Cutscene1("The Beyond"));
	m_scenes.push_back(new LevelOne("The Beyond"));
	m_scenes.push_back(new CreditScene("The Beyond"));
	m_scenes.push_back(new Controls("The Beyond"));
	m_scenes.push_back(new LevelTwo("The Beyond"));
	m_scenes.push_back(new Cutscene2("The Beyond"));
	m_scenes.push_back(new Credits2("The Beyond"));
	
	//preloads all scenes
	m_scenes[0]->InitTexture();
	m_scenes[1]->InitTexture();
	m_scenes[2]->InitTexture();
	m_scenes[3]->InitTexture();
	m_scenes[4]->InitTexture();
	m_scenes[5]->InitTexture();
	m_scenes[6]->InitTexture();
	m_scenes[7]->InitTexture();
	m_scenes[8]->InitTexture();
	m_scenes[9]->InitTexture();

	//Sets active scene reference to our scene
	m_activeScene = m_scenes[0];

	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	//Sets m_register to point to the register in the active scene
	m_register = m_activeScene->GetScene();

	BackEnd::SetWindowName(m_activeScene->GetName());

	PhysicsSystem::Init();
}

bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with activescene clearColor
		m_window->Clear(m_activeScene->GetClearColor());
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();

		//Flips the windows
		m_window->Flip();

		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	//Update Physics System
	PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());

	unsigned int index = m_activeScene->ChangeScene();
	if (index != -1) {
		m_activeScene->Unload();
		PhysicsBody::m_bodiesToDelete.clear();
		//PhysicsSystem::CleanupBodies();
		m_activeScene = m_scenes[index];
		//reload the scene if we need to enter it again
		if (!m_activeScene->is_done)
		{
			m_activeScene->InitTexture();
		}
		else
		{
			//PhysicsSystem::CleanupBodies();
		}
		m_activeScene->InitScene(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
		m_register = m_activeScene->GetScene();
		m_window->SetWindowName(m_activeScene->GetName());
		PhysicsSystem::Init();
		//PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());
	}
	//Updates the active scene
	m_activeScene->Update();

	{
		//Creates a view consisting of all entityies containing horizontal scroll
		auto view = m_register->view<HorizontalScroll>();

		//Loops through all the entities within view2_1
		for (auto entity : view)
		{
			//Grabs a reference to the scroll component
			auto& scroll = view.get(entity);

			//Updates the scroll
			scroll.Update();
		}
	}

	{
		//Creates a view of all entities consisting of vertical scroill
		auto view = m_register->view<VerticalScroll>();

		//Loops through all the entities within view2
		for (auto entity : view)
		{
			//Grabs a reference to the Camera component (in x entity)
			auto& scroll = view.get(entity);

			//Updates the camera
			scroll.Update();
		}
	}
}

void Game::GUI()
{

	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);

	m_activeScene->GUI();

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions 
	GamepadInput();

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	//Gamepad button stroked (pressed)
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			//If the controller is connected, we run the different input types
			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}

void Game::GamepadStroke(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadStroke(con);
}

void Game::GamepadUp(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadUp(con);
}

void Game::GamepadDown(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadDown(con);
}

void Game::GamepadStick(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadStick(con);
}

void Game::GamepadTrigger(XInputController * con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadTrigger(con);
}

void Game::KeyboardHold()
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		m_activeScene->KeyboardHold();
	}
}

void Game::KeyboardDown()
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		m_activeScene->KeyboardDown();
	}
}

void Game::KeyboardUp()
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		m_activeScene->KeyboardUp();

		if (Input::GetKeyUp(Key::F1))
		{
			m_guiActive = !m_guiActive;
		}
		if (Input::GetKeyUp(Key::P))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}
	}
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseMotion(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseClick(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
		ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
		ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
		ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
	}

	//Resets the enabled flag
	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseWheel(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}