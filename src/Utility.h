#pragma once


class Utility
{
public:

	RE::TESGlobal* Survival_ModeToggle;
	RE::TESGlobal* Survival_ModeEnabled;
	RE::TESGlobal* Survival_ModeEnabledShared;
	RE::TESGlobal* Survival_ModeCanBeEnabled;

	RE::SpellItem* Survival_abLowerCarryWeightSpell;
	RE::SpellItem* Survival_abRacialNord;
	RE::SpellItem* Survival_abRacialAltmer;
	RE::SpellItem* Survival_abRacialOrc;
	RE::SpellItem* Survival_abRacialBosmer;
	RE::SpellItem* Survival_abRacialDunmer;
	RE::SpellItem* Survival_abRacialKhajiit;
	RE::SpellItem* Survival_abRacialArgonianRawMeat;
	RE::SpellItem* Survival_abRacialKhajiitRawMeat;
	RE::SpellItem* Survival_abWarmthTorch;
	RE::SpellItem* Survival_OverencumberedSpell;

	RE::BGSListForm* Survival_OblivionCells;
	RE::BGSListForm* Survival_OblivionLocations;
	RE::BGSListForm* Survival_OblivionAreas;

	RE::BGSMessage* Survival_OblivionAreaMessage;

	RE::TESQuest* DA16;

	RE::TESCondition* IsVampireConditions;

	static Utility* GetSingleton()
	{
		static Utility playerStatus;
		return &playerStatus;
	}

	static float GetRandomFloat(float min, float max) 
	{
		return SKSE::stl::RNG::GetSingleton()->Generate<float>(min, max);
	}

	bool IsSurvivalEnabled()
	{
		return (bool)Utility::GetSingleton()->Survival_ModeEnabled->value;
	}

	static void ShowNotification(RE::BGSMessage* msg)
	{
		RE::BSString messageDesc;
		msg->GetDescription(messageDesc, msg);
		RE::DebugNotification(messageDesc.c_str());
	}

	bool SurvivalToggle()
	{
		return (bool)Utility::GetSingleton()->Survival_ModeToggle->value;
	}

	bool PlayerIsInCombat()
	{
		return RE::PlayerCharacter::GetSingleton()->IsInCombat();
	}

	bool PlayerIsInOblivion()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto da16Stage = DA16->GetCurrentStageID();
		if (Survival_OblivionLocations->HasForm(player->GetCurrentLocation()) || 
			Survival_OblivionAreas->HasForm(player->GetWorldspace()) || 
			Survival_OblivionCells->HasForm(player->GetParentCell()) || 
			(da16Stage >= 145 && da16Stage < 160))
		{
			return true;
		}

		return false;
	}

	bool PlayerCanGetWellRested()
	{
		//TODO-Check werewolf as well

		auto player = RE::PlayerCharacter::GetSingleton();
		return !IsVampireConditions->IsTrue(player, nullptr);
	}
};