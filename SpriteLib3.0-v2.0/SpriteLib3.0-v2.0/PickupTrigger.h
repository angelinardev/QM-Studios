#pragma once
#include "Trigger.h"
class PickupTrigger :
    public Trigger
{
public:
	PickupTrigger(int n);

	void OnTrigger() override;

	void OnEnter() override;

	//void OnEnter() override;
	void OnExit() override;

	int type;


};

