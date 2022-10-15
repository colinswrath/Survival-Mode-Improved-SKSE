#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "Needs/NeedCold.h"
#include "Utility.h"

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

void SurvivalMode::SurvivalModeLoopUpdate()
{
	auto utility = Utility::GetSingleton();
	utility->Survival_ModeCanBeEnabled->value = 1.0f;	//TODO-Move this elsewhere

	if (!CheckOblivionStatus()) {
		if (utility->IsSurvivalEnabled() && !utility->SurvivalToggle()) {
			StopSurvivalMode();
		} else if (!utility->IsSurvivalEnabled() && utility->SurvivalToggle()) {
			StartSurvivalMode();	
		} else if (utility->IsSurvivalEnabled()) {
			SendAllNeedsUpdate();
		}
	}
}

void SurvivalMode::StartSurvivalMode()
{
	AddPlayerSpellPerks();
	InitializeAllNeeds();
	Utility::GetSingleton()->Survival_ModeEnabled->value = 1.0f;
	Utility::GetSingleton()->Survival_ModeEnabledShared->value = 1.0f;
}

void SurvivalMode::StopSurvivalMode()
{
	StopAllNeeds();
	RemovePlayerSpellPerks();
	Utility::GetSingleton()->Survival_ModeEnabled->value = 0;
	Utility::GetSingleton()->Survival_ModeEnabledShared->value = 0;
}

void SurvivalMode::InitializeAllNeeds()
{
	logger::info("Initializing all needs");

	NeedHunger::GetSingleton()->InitializeNeed();
	NeedExhaustion::GetSingleton()->InitializeNeed();
	NeedCold::GetSingleton()->InitializeNeed();

	logger::info("Needs initialized");
}

void SurvivalMode::SendAllNeedsUpdate()
{
	NeedHunger::GetSingleton()->OnUpdatePass();
	NeedExhaustion::GetSingleton()->OnUpdatePass();
	NeedCold::GetSingleton()->OnUpdatePass();
}

void SurvivalMode::StopAllNeeds()
{
	logger::info("Stopping all needs");

	NeedHunger::GetSingleton()->StopNeed();
	NeedExhaustion::GetSingleton()->StopNeed();
	NeedCold::GetSingleton()->StopNeed();
	logger::info("Needs stopped");
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

void SurvivalMode::AddPlayerSpellPerks() 
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto utility = Utility::GetSingleton();

	player->AddSpell(utility->Survival_abLowerCarryWeightSpell);
	player->AddSpell(utility->Survival_abRacialNord);
	player->AddSpell(utility->Survival_abRacialAltmer);
	player->AddSpell(utility->Survival_abRacialOrc);
	player->AddSpell(utility->Survival_abRacialBosmer);
	player->AddSpell(utility->Survival_abRacialDunmer);
	player->AddSpell(utility->Survival_abRacialKhajiit);
	player->AddSpell(utility->Survival_abRacialArgonianRawMeat);
	player->AddSpell(utility->Survival_abRacialKhajiitRawMeat);
	player->AddSpell(utility->Survival_abWarmthTorch);
	player->AddSpell(utility->Survival_OverencumberedSpell);
}

void SurvivalMode::RemovePlayerSpellPerks()
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto utility = Utility::GetSingleton();

	player->RemoveSpell(utility->Survival_abLowerCarryWeightSpell);
	player->RemoveSpell(utility->Survival_abRacialNord);
	player->RemoveSpell(utility->Survival_abRacialAltmer);
	player->RemoveSpell(utility->Survival_abRacialOrc);
	player->RemoveSpell(utility->Survival_abRacialBosmer);
	player->RemoveSpell(utility->Survival_abRacialDunmer);
	player->RemoveSpell(utility->Survival_abRacialKhajiit);
	player->RemoveSpell(utility->Survival_abRacialArgonianRawMeat);
	player->RemoveSpell(utility->Survival_abRacialKhajiitRawMeat);
	player->RemoveSpell(utility->Survival_abWarmthTorch);
	player->RemoveSpell(utility->Survival_OverencumberedSpell);
}

bool SurvivalMode::CheckOblivionStatus()
{
	auto utility = Utility::GetSingleton();

	bool oblivion = utility->PlayerIsInOblivion();
	if (oblivion && utility->SMI_WasInOblivion->value == 0.0f) {
		ShowNotification(utility->Survival_OblivionAreaMessage);
		StopAllNeeds();
		utility->SMI_WasInOblivion->value = 1.0f;
	} else if (!oblivion && utility->SMI_WasInOblivion->value == 1.0f) {
		InitializeAllNeeds();
		utility->SMI_WasInOblivion->value = 0.0f;
	}

	return oblivion;
}
