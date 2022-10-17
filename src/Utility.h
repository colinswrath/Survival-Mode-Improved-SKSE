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

	RE::TESGlobal* SMI_WasInOblivion;

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

	RE::Effect* Survival_FireCloakFreezingWaterDesc;

	RE::TESCondition* IsVampireConditions;

	RE::BGSMessage* Survival_OblivionAreaMessage;
	RE::TESQuest* DA16;

	uintptr_t PlayerSingletonAddress;
	uintptr_t UISingletonAddress;
	uintptr_t CalendarSingletonAddress;
	uintptr_t MenuControlsSingletonAddress;
	uintptr_t GetWarmthRatingAddress;

	static Utility* GetSingleton()
	{
		static Utility playerStatus;
		return &playerStatus;
	}

	AREA_TYPE GetCurrentAreaType()
	{
		auto player = GetPlayer();
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
	}

	static RE::PlayerCharacter* GetPlayer()
	{
		REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ Utility::GetSingleton()->PlayerSingletonAddress };
		return singleton->get();
	}

	static RE::UI* GetUI()
	{
		REL::Relocation<RE::NiPointer<RE::UI>*> singleton{ Utility::GetSingleton()->UISingletonAddress };
		return singleton->get();
	}

	static RE::Calendar* GetCalendar()
	{
		REL::Relocation<RE::NiPointer<RE::Calendar>*> singleton{ Utility::GetSingleton()->CalendarSingletonAddress };
		return singleton->get();
	}

	static RE::MenuControls* GetMenuControls()
	{
		REL::Relocation<RE::NiPointer<RE::MenuControls>*> singleton{ Utility::GetSingleton()->MenuControlsSingletonAddress };
		return singleton->get();
	}

	static float GetRandomFloat(float min, float max) 
	{
		return SKSE::stl::RNG::GetSingleton()->Generate<float>(min, max);
	}

	bool IsSurvivalEnabled()
	{
		return Survival_ModeEnabled->value;
	}

	static void ShowNotification(RE::BGSMessage* msg)
	{
		RE::BSString messageDesc;
		msg->GetDescription(messageDesc, msg);
		RE::DebugNotification(messageDesc.c_str());
	}

	bool SurvivalToggle()
	{
		return Survival_ModeToggle->value;
	}

	bool PlayerHasFlameCloak()
	{
		auto player = GetPlayer();

		auto activeEffects = player->AsMagicTarget()->GetActiveEffectList();
		RE::EffectSetting* setting = nullptr;
		for (auto& effect : *activeEffects) {
			setting = effect ? effect->GetBaseObject() : nullptr;
			if (setting) {
				if (setting->data.archetype == RE::EffectSetting::Archetype::kCloak && setting->data.resistVariable == RE::ActorValue::kResistFire) {
					
					return true;
				}
			}
		}
		
		return false;
	}

	bool PlayerIsInOblivion()
	{
		auto player = GetPlayer();
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
		return !IsVampireConditions->IsTrue(GetPlayer(), nullptr);
	}

	bool PlayerIsInJail()
	{
		//auto player = RE::PlayerCharacter::GetSingleton();
	}

	static bool PlayerIsBeastRace()
	{
		auto menuControls = Utility::GetSingleton()->GetMenuControls();
		return menuControls->InBeastForm();
	}

	static bool IsPlayerInDialogue()
	{
		return Utility::GetSingleton()->GetUI()->IsMenuOpen("Dialogue Menu");
	}

	static bool IsOnFlyingMount(RE::Actor* a_actor)
	{
		using func_t = decltype(&Utility::IsOnFlyingMount);
		static REL::Relocation<func_t> func{ RELOCATION_ID(36877, 37901) };
		return func(a_actor);
	}

	static float GetWarmthRating(RE::Actor* actor)
	{
		using func_t = decltype(&Utility::GetWarmthRating);
		REL::Relocation<func_t> func{ Utility::GetSingleton()->GetWarmthRatingAddress };
		return func(actor);
	}
};
