#pragma once
#include "Scene.h"
class Cutscene1 :
    public Scene
{
public:
	Cutscene1(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;
	void InitTexture() override;

	int ChangeScene() override;

private:
	int selection;
	int scene;
	int background;

};

