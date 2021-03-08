#pragma once
#include "ECS.h"
class CanDamage
{
public:
	void InitBody(PhysicsBody& b, AnimationController& a);
	bool m_candamage = true;
	bool m_stun = false;
	bool m_suck = false;
	int hp = 100;
	bool m_canbestun = false;
	bool is_boo = false;

	void Walk();
	void Attack();
private:
	PhysicsBody body;
	AnimationController anims;
};

