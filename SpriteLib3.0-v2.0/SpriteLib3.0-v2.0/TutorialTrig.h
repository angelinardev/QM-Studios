#pragma once
#include "Trigger.h"
class TutorialTrig :
    public Trigger
{
public:
	TutorialTrig(int n);

	void OnTrigger() override;

	void OnEnter() override;

	//void OnEnter() override;
	void OnExit() override;

	int entity;

};

