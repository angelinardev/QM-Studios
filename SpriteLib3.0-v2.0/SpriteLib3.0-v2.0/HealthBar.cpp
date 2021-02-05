#include "HealthBar.h"
#include "Utilities.h"

void HealthBar::UpdateHealthBar(int hb, int hbb, int ui)
{
	auto& hbui = ECS::GetComponent<Transform>(hb);
	auto& hbspr = ECS::GetComponent<Sprite>(hb);
	auto& hbbui = ECS::GetComponent<Transform>(hbb);
	auto& cameraH = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	auto& cameraV = ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera());
	auto& uibg = ECS::GetComponent<Transform>(ui);

	auto& player = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	hbbui.SetPositionX(cameraH.GetCam()->GetPosition().x - 80);
	hbbui.SetPositionY(cameraV.GetCam()->GetPosition().y + 70);
	
	uibg.SetPositionX(cameraH.GetCam()->GetPosition().x);
	uibg.SetPositionY(cameraV.GetCam()->GetPosition().y + 72);
	int hboffset = 0;
	if (MainEntities().Health() >= 0) {
		hboffset = (100 - MainEntities().Health()) / 4;
		hbspr.SetWidth(MainEntities().Health() / 2);
	}
	else
	{
		hboffset = (100 - 0) / 4;
		hbspr.SetWidth(0);
	}

	//int hboffset = (100 - MainEntities().Health()) / 4;
	
	hbui.SetPositionX((cameraH.GetCam()->GetPosition().x - 80));
	hbui.SetPositionY((cameraV.GetCam()->GetPosition().y + 70));
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





