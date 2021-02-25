#pragma once
#include "Scene.h"
class Cutscene1 :
    public Scene
{
public:
	Cutscene1(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	int ChangeScene() override;

	int selection;
	int scene;

};

