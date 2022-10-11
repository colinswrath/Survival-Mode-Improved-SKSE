#pragma once

enum class AREA_TYPE
{
	kAreaTypeChillyInterior = -1,
	kAreaTypeInterior = 0,
	kAreaTypeWarm = 1,
	kAreaTypeCool = 2,
	kAreaTypeFreezing = 3,
	kAreaTypeReach = 4
};

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

	RE::BGSListForm* Survival_InteriorAreas;
	RE::BGSListForm* Survival_ColdInteriorLocations;
	RE::BGSListForm* Survival_ColdInteriorCells;

	RE::TESCondition* IsInWarmArea;
	RE::TESCondition* IsInCoolArea;
	RE::TESCondition* IsInFreezingArea;
	RE::TESCondition* IsInFallForestFreezingArea;
	RE::TESCondition* IsInPineForestFreezingArea;
	RE::TESCondition* IsInReachArea;

	RE::TESCondition* IsVampireConditions;

	RE::BGSMessage* Survival_OblivionAreaMessage;
	RE::TESQuest* DA16;

	static Utility* GetSingleton()
	{
		static Utility playerStatus;
		return &playerStatus;
	}

	AREA_TYPE GetCurrentAreaType()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto playerParentCell = player->GetParentCell();

		if (playerParentCell->IsInteriorCell() || Survival_InteriorAreas->HasForm(player->GetWorldspace())) {
			if (Survival_ColdInteriorLocations->HasForm(playerParentCell) || Survival_ColdInteriorCells->HasForm(playerParentCell)) {
				return AREA_TYPE::kAreaTypeChillyInterior;
			} else {
				return AREA_TYPE::kAreaTypeInterior;
			}
		} else if (IsInPineForestFreezingArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeFreezing;
		} else if (IsInFallForestFreezingArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeFreezing;
		} else if (IsInWarmArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeWarm;
		} else if (IsInCoolArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeCool;
		} else if (IsInFreezingArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeFreezing;
		} else if (IsInReachArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeReach;
		} else {
			return AREA_TYPE::kAreaTypeCool;
		}
	static float GetRandomFloat(float min, float max) 
	{
		return SKSE::stl::RNG::GetSingleton()->Generate<float>(min, max);
	}

	bool IsSurvivalEnabled()
	{
		return (bool)Survival_ModeEnabled->value;
	}

	static void ShowNotification(RE::BGSMessage* msg)
	{
		RE::BSString messageDesc;
		msg->GetDescription(messageDesc, msg);
		RE::DebugNotification(messageDesc.c_str());
	}

	bool SurvivalToggle()
	{
		return (bool)Survival_ModeToggle->value;
	}

	bool PlayerIsInCombat()
	{
		return RE::PlayerCharacter::GetSingleton()->IsInCombat();
	}

	bool PlayerIsInFreezingWater()
	{

	}

	bool PlayerIsInOblivion()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto da16Stage = DA16->GetCurrentStageID();
		if (Survival_OblivionLocations->HasForm(player->GetCurrentLocation()) ||
			Survival_OblivionAreas->HasForm(player->GetWorldspace()) ||
			Survival_OblivionCells->HasForm(player->GetParentCell()) ||
			(da16Stage >= 145 && da16Stage < 160)) {
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

	static float GetWarmthRating(RE::Actor* actor)
	{
		using func_t = decltype(&PlayerStatus::GetWarmthRating);
		REL::Relocation<func_t> func{ RELOCATION_ID(25834, 26394) };
		return func(actor);
	}

	inline static REL::Relocation<decltype(GetWarmthRating)> _GetWarmthRating;
};
