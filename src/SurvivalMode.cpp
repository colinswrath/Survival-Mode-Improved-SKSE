#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"
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
	PlayerStatus::GetSingleton()->Survival_ModeCanBeEnabled->value = 1.0f;

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

/// <summary>
/// Send initialization to all needs
/// </summary>
void SurvivalMode::InitializeAllNeeds()
{
	NeedHunger::GetSingleton()->InitializeNeed();
	//Fatigue
	//Cold
	PlayerStatus::GetSingleton()->Survival_ModeEnabled->value = 1.0f;
	PlayerStatus::GetSingleton()->Survival_ModeEnabledShared->value = 1.0f;
}

/// <summary>
/// Send stop to all needs
/// </summary>
void SurvivalMode::StopAllNeeds()
{
	NeedHunger::GetSingleton()->StopNeed();
	//Fatigue
	//Cold
	PlayerStatus::GetSingleton()->Survival_ModeEnabled->value = 0;
	PlayerStatus::GetSingleton()->Survival_ModeEnabledShared->value = 0;
}

/// <summary>
/// Send update to all needs
/// </summary>
void SurvivalMode::SendAllNeedsUpdate()
{
	NeedHunger::GetSingleton()->OnUpdateNeed();
	//Fatigue
	//Cold
}

bool SurvivalMode::InstallUpdateHook()
{
	auto& trampoline = SKSE::GetTrampoline();
	_OnUpdate = trampoline.write_call<5>(Hooks::On_Update_Hook.address(), OnUpdate);
	logger::info("Installed update hook");
	return true;
}

