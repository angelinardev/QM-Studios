#include "Invisibility.h"
#include "ECS.h"


void Invisibility::update_invisible()
{
	if (is_invisible)
	{
		ECS::GetComponent<Sprite>(ent).SetTransparency(0);
	}
	else
	{
		ECS::GetComponent<Sprite>(ent).SetTransparency(1);
	}
}

void Invisibility::set_entity(int entity)
{
	ent = entity;
}
