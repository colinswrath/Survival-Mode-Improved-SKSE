#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
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

/// <summary>
/// Main update loop
/// </summary>
void SurvivalMode::SurvivalModeLoopUpdate()
{
	auto playerStatus = Utility::GetSingleton();
	playerStatus->Survival_ModeCanBeEnabled->value = 1.0f;

	if (playerStatus->PlayerIsInOblivion()) {
		ShowNotification(playerStatus->Survival_OblivionAreaMessage);
		StopAllNeeds();
	} else {

		if (playerStatus->IsSurvivalEnabled() && !playerStatus->SurvivalToggle()) {
			StopSurvivalMode();
		} else if (!playerStatus->IsSurvivalEnabled() && playerStatus->SurvivalToggle()) {
			StartSurvivalMode();	
		} else if (playerStatus->IsSurvivalEnabled()) {
			SendAllNeedsUpdate();
		}
	}
}

void SurvivalMode::StartSurvivalMode()
{
	AddPlayerSpellPerks();
	InitializeAllNeeds();
	SendAllNeedsUpdate();
}

void SurvivalMode::StopSurvivalMode()
{
	StopAllNeeds();
	RemovePlayerSpellPerks();
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

	Utility::GetSingleton()->Survival_ModeEnabled->value = 1.0f;
	Utility::GetSingleton()->Survival_ModeEnabledShared->value = 1.0f;

	logger::info("Needs initialized");
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

/// <summary>
/// Send stop to all needs
/// </summary>
void SurvivalMode::StopAllNeeds()
{
	logger::info("Stopping all needs");

	NeedHunger::GetSingleton()->StopNeed();
	NeedExhaustion::GetSingleton()->StopNeed();
	//Cold

	Utility::GetSingleton()->Survival_ModeEnabled->value = 0;
	Utility::GetSingleton()->Survival_ModeEnabledShared->value = 0;
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

	player->AddSpell(Utility::GetSingleton()->Survival_abLowerCarryWeightSpell);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialNord);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialAltmer);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialOrc);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialBosmer);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialDunmer);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialKhajiit);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialArgonianRawMeat);
	player->AddSpell(Utility::GetSingleton()->Survival_abRacialKhajiitRawMeat);
	player->AddSpell(Utility::GetSingleton()->Survival_abWarmthTorch);
	player->AddSpell(Utility::GetSingleton()->Survival_OverencumberedSpell);
}

void SurvivalMode::RemovePlayerSpellPerks()
{
	auto player = RE::PlayerCharacter::GetSingleton();

	player->RemoveSpell(Utility::GetSingleton()->Survival_abLowerCarryWeightSpell);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialNord);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialAltmer);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialOrc);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialBosmer);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialDunmer);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialKhajiit);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialArgonianRawMeat);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abRacialKhajiitRawMeat);
	player->RemoveSpell(Utility::GetSingleton()->Survival_abWarmthTorch);
	player->RemoveSpell(Utility::GetSingleton()->Survival_OverencumberedSpell);
}

