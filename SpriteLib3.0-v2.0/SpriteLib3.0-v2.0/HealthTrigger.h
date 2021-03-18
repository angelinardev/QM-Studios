#pragma once
#include "Trigger.h"
class HealthTrigger :
	public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;

	//void OnEnter() override;
	void OnExit() override;
};

