#pragma once
#include "Scene.h"
#include "ToneFire/ToneFire.h"
class Credits2 :
	public Scene
{
public:
	Credits2(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;
	void InitTexture() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	int selectionCounter = 4;
	int selector = 0;
	int background = 0;
	int selection = -1;
	ToneFire::FMODCore Fmod{};
	ToneFire::CoreSound Sound{ "Menusong.mp3", FMOD_2D | FMOD_LOOP_NORMAL };

};

