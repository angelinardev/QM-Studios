#pragma once
#include "Scene.h"
#include "ToneFire/ToneFire.h"
class Controls :
    public Scene
{
public:
	Controls(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;
	void InitTexture() override;

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
	ToneFire::FMODCore Fmod{};
	ToneFire::CoreSound Sound{ "Menusong.mp3",FMOD_2D | FMOD_LOOP_NORMAL };
};

