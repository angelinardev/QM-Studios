#pragma once
#include "Scene.h"
#include "ToneFire/ToneFire.h"

class TitleScreen : public Scene
{
public:
	TitleScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
	void InitTexture() override;

	void Update() override;

	int ChangeScene() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void setSelect(int);
	
	int setY(int);
	int setW(int);
	

	int selectionCounter = 4;
	int selector = 0;
	int background = 0;
	int selection = -1;
	int firef = 0;
	ToneFire::FMODCore Fmod{};
	ToneFire::CoreSound Sound{ "Menusong.mp3",FMOD_2D | FMOD_LOOP_NORMAL };

};

