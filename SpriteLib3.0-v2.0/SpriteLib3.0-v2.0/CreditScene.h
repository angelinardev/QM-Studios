#pragma once
#include "Scene.h"
class CreditScene :
    public Scene
{
public:
	CreditScene(std::string name);

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

};

