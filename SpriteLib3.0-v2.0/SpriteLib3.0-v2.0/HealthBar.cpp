#include "HealthBar.h"
#include "Utilities.h"

void HealthBar::UpdateHealthBar(int hb, int ui)
{
	auto& hbui = ECS::GetComponent<Transform>(hb);
	auto& hbspr = ECS::GetComponent<Sprite>(hb);
	//auto& hbbui = ECS::GetComponent<Transform>(hbb);
	auto& cameraH = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	auto& cameraV = ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera());
	auto& uibg = ECS::GetComponent<Transform>(ui);

	auto& player = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	//hbbui.SetPositionX(cameraH.GetCam()->GetPosition().x - 80);
	//hbbui.SetPositionY(cameraV.GetCam()->GetPosition().y + 70);
	
	uibg.SetPositionX(cameraH.GetCam()->GetPosition().x);
	uibg.SetPositionY(cameraV.GetCam()->GetPosition().y + 72);
	int hboffset = 0;
	std::string fileName;
	if (MainEntities().Health() >= 0) {
		//hboffset = (100 - MainEntities().Health()) / 4;
		//hbspr.SetWidth(MainEntities().Health() / 2);
		if (MainEntities().Health() > 75)
		{
			fileName = "Health.png";
		}
		else if (MainEntities().Health() > 50)
		{
			fileName = "Health1.png";
		}
		else if (MainEntities().Health() > 25)
		{
			fileName = "Health2.png";
		}
		else if (MainEntities().Health() > 0)
		{
			fileName = "Health3.png";
		}
	}
	else
	{
		hboffset = (100 - 0) / 4;
		fileName = "Health4.png";
	}
	hbspr.LoadSprite(fileName, 40, 10);

	//int hboffset = (100 - MainEntities().Health()) / 4;
	
	hbui.SetPositionX((cameraH.GetCam()->GetPosition().x - 90));
	hbui.SetPositionY((cameraV.GetCam()->GetPosition().y + 70));

	
}

void HealthBar::UpdatePowers(int p_count) //int p
{
	auto& p_countui = ECS::GetComponent<Transform>(p_count);
	auto& p_cspr = ECS::GetComponent<Sprite>(p_count);
	//auto& pui = ECS::GetComponent<Transform>(p);
	//auto& pspr = ECS::GetComponent<Sprite>(p);
	auto& cameraH = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	auto& cameraV = ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera());

	auto& playerb = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	//testing values, change later
	p_countui.SetPositionX(playerb.GetBody()->GetWorldCenter().x - 80);
	p_countui.SetPositionY(playerb.GetBody()->GetWorldCenter().y + 70);

	int power_count = 0;
	auto& powers = ECS::GetComponent<Player_Power>(MainEntities::MainPlayer());
	for (int i = 0; i < MainEntities::Powerups().size(); i++)
	{
		if (MainEntities::Powerups()[i])
		{
			power_count++; //increase count
			if (powers.m_power[0])
			{
				//show this power is active in UI (change sprite)
			}
			if (powers.m_power[1])
			{
				//show this power is active in UI (change sprite)
			}
		}
	}
	std::string fileName;
	switch (power_count)
	{
	case 0:
		fileName = "page0.png";
		break;
	case 1: //display different count based on current amount obtained

		fileName = "page1.png"; //filename
		break;

	case 2:
		fileName = "page2.png";
		break;
	}
	p_cspr.LoadSprite(fileName, 15, 15);

	p_countui.SetPositionX((cameraH.GetCam()->GetPosition().x - 50));
	p_countui.SetPositionY((cameraV.GetCam()->GetPosition().y + 71));
}

//void HealthBar::UpdateGhostCounter(std::vector<int> ghosts, int fillColour, int backColour)
//{
//	
//
//	auto& fcui = ECS::GetComponent<Transform>(fillColour);
//	auto& fcspr = ECS::GetComponent<Sprite>(fillColour);
//	auto& bcui = ECS::GetComponent<Transform>(backColour);
//	auto& bcspr = ECS::GetComponent<Sprite>(backColour);
//	auto& cameraH = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
//	auto& cameraV = ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera());
//	int ghostscaptured = 0;
//	if (MainEntities::Captured() >= 0) {
//		ghostscaptured = MainEntities::Captured();
//	}
//	else
//	{
//		ghostscaptured = 0;
//	}
//
//	if (MainEntities::Captured() <= ghosts.size()) {
//		ghostscaptured = MainEntities::Captured();
//
//	}
//	else
//	{
//		ghostscaptured = ghosts.size();
//	}
//	
//	int ghostphoto = (bcspr.GetWidth() / (ghosts.size()));
//	
//	int fcoffset = ((bcspr.GetWidth()/2) - (ghostscaptured*(ghostphoto/2)));
//	
//	//(ghostscaptured * ghostphoto) - (((ghosts.size() / 2) + 1) * ghostphoto) - (ghostscaptured * (ghostphoto/2));
//					//((ghostscaptured * ghostphoto) - (((ghosts.size()/2)+1) * ghostphoto)) - (((ghostscaptured) * ghostphoto)/2);
//	
//
//
//	fcspr.SetWidth((ghostscaptured) * ghostphoto);
//	double extra = 0;
//	for (int i = 0; i < ghostscaptured; i++) {
//		extra += 0.5;
//	}
//	bcspr.SetWidth(ghosts.size() * ghostphoto);
//	fcui.SetPositionX((cameraH.GetCam()->GetPosition().x + (60 + (int)extra)) - fcoffset);
//	fcui.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);
//
//	bcui.SetPositionX(cameraH.GetCam()->GetPosition().x + 60);
//	bcui.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);
//
//	int index = 0;
//	for (int i : ghosts) {
//		//st position
//		auto& ghostFilter = ECS::GetComponent<Transform>(i);
//		ghostFilter.SetPositionY(cameraV.GetCam()->GetPosition().y - 72);
//		ghostFilter.SetPositionX((cameraH.GetCam()->GetPosition().x + 60) + ((bcspr.GetWidth()/2) - (ghostphoto/2) - (index*(ghostphoto))));
//		index++;
//		
//	}
//
//}
//
//void HealthBar::UpdateVignette()
//{
//	VignetteEffect* ve = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
//	//float innerRadius = ve->GetInnerRadius();
//	//float outerRadius = ve->GetOuterRadius();
//	//float opacity = ve->GetOpacity();
//
//	ve->SetInnerRadius(3.f);
//	ve->SetOuterRadius(6.f);
//	ve->SetOpacity(0.5f);
//}





