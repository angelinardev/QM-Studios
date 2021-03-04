#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "ToneFire/ToneFire.h"
#include <string>

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;
	void InitTexture() override;
	int ChangeScene() override;
	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;
	int p_entity;

	float speed = 0;
	float theta = 0;

	//testing purposes
	int facing = 0;
	
	bool dashcooldown = true;
	double dash_timer;
	clock_t dashtime;
	float cooldown = 1.5f;
	int jspeed = 0;
	double jump_timer;
	clock_t jumptime;
	bool jumphold = false;

	bool jump_high = false;

	int test_e1 = 0;

	int selection = -1;
	//tutorial controls
	int  tut1;
	int tut2;
	int tut3;

	ToneFire::FMODCore Fmod{};
	ToneFire::CoreSound Sound{ "tutorial slow.mp3",FMOD_2D | FMOD_LOOP_NORMAL };
	int healthBar;
	 int healthBarBack;
	 int ghostBar;
	 int ghostBarBack;
	int pcount;
	HealthBar hb;


};
