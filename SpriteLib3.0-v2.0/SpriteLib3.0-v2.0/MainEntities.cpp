#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
std::vector<bool> MainEntities::powerups = {false, false, false};

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

std::vector<bool> MainEntities::Powerups()
{
	return powerups;
}


void MainEntities::Powerups(std::vector<bool> p)
{
	powerups = p;
}
