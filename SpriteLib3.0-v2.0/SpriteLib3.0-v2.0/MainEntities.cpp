#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::health = 100;

int MainEntities::MainCamera()
{
	return m_mainCamera;
}

int MainEntities::MainPlayer()
{
	return m_mainPlayer;
}

void MainEntities::MainCamera(int main)
{
	m_mainCamera = main;
}

void MainEntities::MainPlayer(int main)
{
	m_mainPlayer = main;
}
int MainEntities::Health()
{
	return health;
}
void MainEntities::Health(int n)
{
	health = n;
	if (health > 100)
	{
		health = 100;
	}
	if (health <= 0)
	{
		//idk
	}
}