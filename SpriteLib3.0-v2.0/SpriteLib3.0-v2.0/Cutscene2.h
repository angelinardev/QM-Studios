#pragma once
#include "Scene.h"
class Cutscene2 :
    public Scene
{
public:
	Cutscene2(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;
	void InitTexture() override;

	int ChangeScene() override;
	//Input overrides
	void KeyboardDown() override;
private:
	int selection;
	int scene;
	int background;
};

