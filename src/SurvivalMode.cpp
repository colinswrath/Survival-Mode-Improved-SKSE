#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "Needs/NeedCold.h"
#include "Utility.h"

std::int32_t SurvivalMode::OnUpdate()
{ 
	if (!Utility::GetUI()->GameIsPaused()) {
		
		if (g_deltaTime > 0) {
			lastTime += g_deltaTime;
			if (lastTime >= 1.0f) {
				SurvivalModeLoopUpdate();
				lastTime = 0;
			}
		}
	}

	return _OnUpdate();
}

void SurvivalMode::SurvivalModeLoopUpdate()
{
	auto utility = Utility::GetSingleton();
	utility->Survival_ModeCanBeEnabled->value = 1.0f;

	if (utility->IsSurvivalEnabled() && !utility->SurvivalToggle()) {
		StopSurvivalMode();
	} else if (!CheckOblivionStatus() && !CheckJailStatus()) {
		if (!utility->IsSurvivalEnabled() && utility->SurvivalToggle()) {
			StartSurvivalMode();
		} else if (utility->IsSurvivalEnabled()) {
			SendAllNeedsUpdate();
		}
	}
}

void SurvivalMode::StartSurvivalMode()
{
	auto playerParentCell = Utility::GetPlayer()->GetParentCell();
	if (RE::ControlMap::GetSingleton()->IsMainFourControlsEnabled() && !playerParentCell->IsInteriorCell()) {
		auto utility = Utility::GetSingleton();
		AddPlayerSpellPerks();
		SendAllNeedsUpdate();
		utility->Survival_ModeEnabled->value = 1.0f;
		utility->Survival_ModeEnabledShared->value = 1.0f;
	}
}

void SurvivalMode::StopSurvivalMode()
{
	auto utility = Utility::GetSingleton();
	StopAllNeeds();
	RemovePlayerSpellPerks();
	utility->Survival_ModeEnabled->value = 0;
	utility->Survival_ModeEnabledShared->value = 0;
}

void SurvivalMode::SendAllNeedsUpdate()
{
	if (Utility::PlayerIsVampire()) {
		NeedHunger::GetSingleton()->StopNeed();
		NeedExhaustion::GetSingleton()->OnUpdatePass();
		NeedCold::GetSingleton()->OnUpdatePass();
	} else if (Utility::PlayerIsLich()) {
		NeedHunger::GetSingleton()->StopNeed();
		NeedExhaustion::GetSingleton()->StopNeed();
		NeedCold::GetSingleton()->StopNeed();
	} else {
		NeedHunger::GetSingleton()->OnUpdatePass();
		NeedExhaustion::GetSingleton()->OnUpdatePass();
		NeedCold::GetSingleton()->OnUpdatePass();
	}
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

bool SurvivalMode::InstallFtMessageHook()
{
	auto& trampoline = SKSE::GetTrampoline();
	_OverwriteFastTravelMessage = trampoline.write_call<5>(Hooks::Fast_Travel_Message_Hook.address(), OverwriteFastTravelMessage);
	logger::info("Installed ft message hook");
	return true;
}

void SurvivalMode::OverwriteFastTravelMessage(const char* a_notification, const char* a_soundToPlay, bool a_cancelIfAlreadyQueued)
{
	if (!Utility::GetUI()->IsMenuOpen(RE::MapMenu::MENU_NAME) || !Utility::DisableFTCheck() || !Utility::GetSingleton()->DisableFastTravel) {
		_OverwriteFastTravelMessage(a_notification, a_soundToPlay, a_cancelIfAlreadyQueued);
	}
}

void SurvivalMode::AddPlayerSpellPerks() 
{
	logger::info("Adding perks");

	auto player = Utility::GetPlayer();
	auto utility = Utility::GetSingleton();

	player->AddSpell(utility->Survival_abLowerCarryWeightSpell);
	player->AddSpell(utility->Survival_abLowerRegenSpell);
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

	player->AddPerk(utility->Survival_TempleBlessingCostPerk);
	logger::info("Perks added");
}

void SurvivalMode::RemovePlayerSpellPerks()
{
	auto player = Utility::GetPlayer();
	auto utility = Utility::GetSingleton();

	Utility::RemoveSurvivalDiseases();

	player->RemoveSpell(utility->Survival_abLowerCarryWeightSpell);
	player->RemoveSpell(utility->Survival_abLowerRegenSpell);
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
	player->RemoveSpell(utility->Survival_OblivionDisplaySpell);

	player->RemovePerk(utility->Survival_TempleBlessingCostPerk);
}

bool SurvivalMode::CheckOblivionStatus()
{
	auto utility = Utility::GetSingleton();

	bool oblivion = utility->PlayerIsInOblivion();
	if (oblivion && !utility->WasInOblivion) {
		Utility::GetPlayer()->AddSpell(utility->Survival_OblivionDisplaySpell);
		ShowNotification(utility->Survival_OblivionAreaMessage);
		StopAllNeeds();
		utility->WasInOblivion = true;
	} else if (!oblivion && utility->WasInOblivion) {
		Utility::GetPlayer()->RemoveSpell(utility->Survival_OblivionDisplaySpell);
		utility->WasInOblivion = false;
	}

	return oblivion;
}

bool SurvivalMode::CheckJailStatus()
{
	bool jail = Utility::PlayerIsInJail();
	if (jail) {
		StopAllNeeds();
	} 

	return jail;
}
