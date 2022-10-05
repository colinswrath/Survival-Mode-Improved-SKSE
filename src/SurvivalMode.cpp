#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "PlayerStatus.h"

std::int32_t SurvivalMode::OnUpdate(std::int64_t a1)
{
	if (!RE::UI::GetSingleton()->GameIsPaused()) {
		
		if (g_deltaTime > 0) {
			lastTime += g_deltaTime;
			if (lastTime >= 1.0f) {
				SurvivalModeLoopUpdate();
				lastTime = 0;
			}
		}
	}

	return _OnUpdate(a1);
}

/// <summary>
/// Main update loop
/// </summary>
void SurvivalMode::SurvivalModeLoopUpdate()
{
	auto playerStatus = PlayerStatus::GetSingleton();
	playerStatus->Survival_ModeCanBeEnabled->value = 1.0f;

	if (playerStatus->PlayerIsInOblivion()) {
		ShowNotification(playerStatus->Survival_OblivionAreaMessage);
		StopAllNeeds();
	} else {

		if (playerStatus->IsSurvivalEnabled() && !playerStatus->SurvivalToggle()) {
			//If SM is on but should be off
			StopAllNeeds();
		} else if (!playerStatus->IsSurvivalEnabled() && playerStatus->SurvivalToggle()) {
			//If SM is off but should be on
			InitializeAllNeeds();
			SendAllNeedsUpdate();
		} else if (playerStatus->IsSurvivalEnabled()) {
			SendAllNeedsUpdate();
		}
	}
}

/// <summary>
/// Send initialization to all needs
/// </summary>
void SurvivalMode::InitializeAllNeeds()
{
	logger::info("Initializing all needs");

	NeedHunger::GetSingleton()->InitializeNeed();
	NeedExhaustion::GetSingleton()->InitializeNeed();
	//Cold

	PlayerStatus::GetSingleton()->Survival_ModeEnabled->value = 1.0f;
	PlayerStatus::GetSingleton()->Survival_ModeEnabledShared->value = 1.0f;

	logger::info("Needs initialized");
}

/// <summary>
/// Send stop to all needs
/// </summary>
void SurvivalMode::StopAllNeeds()
{
	logger::info("Stopping all needs");

	NeedHunger::GetSingleton()->StopNeed();
	NeedExhaustion::GetSingleton()->StopNeed();
	//Cold

	PlayerStatus::GetSingleton()->Survival_ModeEnabled->value = 0;
	PlayerStatus::GetSingleton()->Survival_ModeEnabledShared->value = 0;
	logger::info("Needs stopped");
}

/// <summary>
/// Send update to all needs
/// </summary>
void SurvivalMode::SendAllNeedsUpdate()
{
	NeedHunger::GetSingleton()->OnUpdatePass();
	NeedExhaustion::GetSingleton()->OnUpdatePass();
	//Cold
}

void SurvivalMode::ShowNotification(RE::BGSMessage* msg)
{
	RE::BSString messageDesc;
	msg->GetDescription(messageDesc, msg);
	RE::DebugNotification(messageDesc.c_str());
}

bool SurvivalMode::InstallUpdateHook()
{
	auto& trampoline = SKSE::GetTrampoline();
	_OnUpdate = trampoline.write_call<5>(Hooks::On_Update_Hook.address(), OnUpdate);
	logger::info("Installed update hook");
	return true;
}

