#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "Needs/NeedCold.h"
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
	NeedCold::GetSingleton()->InitializeNeed();

	PlayerStatus::GetSingleton()->Survival_ModeEnabled->value = 1.0f;
	PlayerStatus::GetSingleton()->Survival_ModeEnabledShared->value = 1.0f;

	logger::info("Needs initialized");
}

/// <summary>
/// Send update to all needs
/// </summary>
void SurvivalMode::SendAllNeedsUpdate()
{
	NeedHunger::GetSingleton()->OnUpdatePass();
	NeedExhaustion::GetSingleton()->OnUpdatePass();
	NeedCold::GetSingleton()->OnUpdatePass();
}

/// <summary>
/// Send stop to all needs
/// </summary>
void SurvivalMode::StopAllNeeds()
{
	logger::info("Stopping all needs");

	NeedHunger::GetSingleton()->StopNeed();
	NeedExhaustion::GetSingleton()->StopNeed();
	NeedCold::GetSingleton()->StopNeed();

	PlayerStatus::GetSingleton()->Survival_ModeEnabled->value = 0;
	PlayerStatus::GetSingleton()->Survival_ModeEnabledShared->value = 0;
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
	auto playerStatus = PlayerStatus::GetSingleton();

	player->AddSpell(playerStatus->Survival_abLowerCarryWeightSpell);
	player->AddSpell(playerStatus->Survival_abRacialNord);
	player->AddSpell(playerStatus->Survival_abRacialAltmer);
	player->AddSpell(playerStatus->Survival_abRacialOrc);
	player->AddSpell(playerStatus->Survival_abRacialBosmer);
	player->AddSpell(playerStatus->Survival_abRacialDunmer);
	player->AddSpell(playerStatus->Survival_abRacialKhajiit);
	player->AddSpell(playerStatus->Survival_abRacialArgonianRawMeat);
	player->AddSpell(playerStatus->Survival_abRacialKhajiitRawMeat);
	player->AddSpell(playerStatus->Survival_abWarmthTorch);
	player->AddSpell(playerStatus->Survival_OverencumberedSpell);
}

void SurvivalMode::RemovePlayerSpellPerks()
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto playerStatus = PlayerStatus::GetSingleton();

	player->RemoveSpell(playerStatus->Survival_abLowerCarryWeightSpell);
	player->RemoveSpell(playerStatus->Survival_abRacialNord);
	player->RemoveSpell(playerStatus->Survival_abRacialAltmer);
	player->RemoveSpell(playerStatus->Survival_abRacialOrc);
	player->RemoveSpell(playerStatus->Survival_abRacialBosmer);
	player->RemoveSpell(playerStatus->Survival_abRacialDunmer);
	player->RemoveSpell(playerStatus->Survival_abRacialKhajiit);
	player->RemoveSpell(playerStatus->Survival_abRacialArgonianRawMeat);
	player->RemoveSpell(playerStatus->Survival_abRacialKhajiitRawMeat);
	player->RemoveSpell(playerStatus->Survival_abWarmthTorch);
	player->RemoveSpell(playerStatus->Survival_OverencumberedSpell);
}

