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

	RE::SpellItem* Survival_OblivionDisplaySpell;

	RE::BGSListForm* Survival_OblivionCells;
	RE::BGSListForm* Survival_OblivionLocations;
	RE::BGSListForm* Survival_OblivionAreas;

	RE::BGSListForm* Survival_InteriorAreas;
	RE::BGSListForm* Survival_ColdInteriorLocations;
	RE::BGSListForm* Survival_ColdInteriorCells;

	RE::BGSListForm* HelpManualPC;
	RE::BGSListForm* HelpManualXBox;

	RE::TESCondition* IsInWarmArea;
	RE::TESCondition* IsInCoolArea;
	RE::TESCondition* IsInFreezingArea;
	RE::TESCondition* IsInFallForestFreezingArea;
	RE::TESCondition* IsInPineForestFreezingArea;
	RE::TESCondition* IsInReachArea;

	RE::TESCondition* WTIsInWarmArea;
	RE::TESCondition* WTIsInCoolArea;
	RE::TESCondition* WTIsInFreezingArea;


	RE::EffectSetting* WerewolfFeedRestoreHealth;
	RE::EffectSetting* DA11AbFortifyHealth;

	RE::TESCondition* IsVampireConditions;
	RE::TESCondition* IsWerewolfConditions;
	RE::TESConditionItem* IsInJailCondition;

	RE::TESConditionItem* AdoptionHomeLocationCond;

	RE::BGSMessage* Survival_OblivionAreaMessage;

	RE::BGSMessage* Survival_HelpSurvivalModeLong;
	RE::BGSMessage* Survival_HelpSurvivalModeLongXbox;

	RE::BGSPerk* Survival_TempleBlessingCostPerk;

	RE::TESQuest* DA16;
	RE::TESQuest* RelationshipMarriageFIN;

	RE::TESQuest* BYOHRelationshipAdoption;
	
	RE::BGSKeyword* LocTypeInn;
	RE::BGSKeyword* LocTypePlayerHouse;

	uintptr_t PlayerSingletonAddress;
	uintptr_t UISingletonAddress;
	uintptr_t CalendarSingletonAddress;
	uintptr_t MenuControlsSingletonAddress;
	uintptr_t GetWarmthRatingAddress;

	bool WasInOblivion = false;

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
		} else if (IsInWarmArea->IsTrue(player, nullptr) || (WTIsInWarmArea && WTIsInWarmArea->IsTrue(player, nullptr))) {
			return AREA_TYPE::kAreaTypeWarm;
		} else if (IsInCoolArea->IsTrue(player, nullptr) || (WTIsInCoolArea && WTIsInCoolArea->IsTrue(player, nullptr))) {
			return AREA_TYPE::kAreaTypeCool;
		} else if (IsInFreezingArea->IsTrue(player, nullptr) || (WTIsInFreezingArea && WTIsInFreezingArea->IsTrue(player, nullptr))) {
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

	static bool PlayerIsWerewolf()
	{
		auto util = Utility::GetSingleton();
		return util->IsWerewolfConditions->IsTrue(util->GetPlayer(), nullptr);
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

	static bool PlayerCanGetWellRested()
	{
		//Vampire or WW or Lich then false

		return !PlayerIsVampire() && !PlayerIsWerewolf();
	}

	static bool PlayerIsVampire()
	{
		auto util = Utility::GetSingleton();
		return util->IsVampireConditions->IsTrue(GetPlayer(), nullptr);
	}

	static bool PlayerIsInJail()
	{
		auto utility = Utility::GetSingleton();
		RE::ConditionCheckParams param(Utility::GetPlayer(), nullptr);
		bool inJail = utility->IsInJailCondition->IsTrue(param);
		return inJail;

	}

	static bool PlayerIsNearSpouse()
	{
		auto relMarriageQuest = Utility::GetSingleton()->RelationshipMarriageFIN;
		auto interestString = RE::BSFixedString("LoveInterest");

		auto aliases = relMarriageQuest->aliases;
		RE::BGSBaseAlias* loveInterestBase = nullptr;

		for (auto alias : aliases) {
			if (alias && alias->aliasName == interestString) {
				loveInterestBase = alias;
				break;
			}
		}

		if (!loveInterestBase) {
			return false;
		}

		const auto loveInterestRef = skyrim_cast<RE::BGSRefAlias*>(loveInterestBase);

		if (!loveInterestRef) {
			return false;
		}

		if (relMarriageQuest->IsRunning() && relMarriageQuest->currentStage >= 10 && (Utility::GetPlayer()->GetCurrentLocation() == loveInterestRef->GetActorReference()->GetCurrentLocation())) {
			return true;
		}
		return false;
	}

	static bool PlayerIsNearAdopted()
	{
		auto util = Utility::GetSingleton();
		auto adoptionQuest = util->BYOHRelationshipAdoption;

		auto child1String = RE::BSFixedString("Child1");
		auto child2String = RE::BSFixedString("Child2");

		auto aliases = adoptionQuest->aliases;

		RE::BGSBaseAlias* child1Base = nullptr;
		RE::BGSBaseAlias* child2Base = nullptr;
		RE::Actor* child1Actor = nullptr;
		RE::Actor* child2Actor = nullptr;
		bool child1Near = false;
		bool child2Near = false;

		for (auto alias : aliases) {
			if (alias && alias->aliasName == child1String) {
				child1Base = alias;
			} else if (alias && alias->aliasName == child2String) {
				child2Base = alias;
			}
		}

		if (!child1Base || child1Base == nullptr) {
			return false;
		}

		const auto child1Ref = skyrim_cast<RE::BGSRefAlias*>(child1Base);

		if (child1Ref) {
			child1Actor = child1Ref->GetActorReference();
			if (child1Actor) {
				child1Near = Utility::GetPlayer()->GetCurrentLocation() == child1Actor->GetCurrentLocation();
			}
		}

		if (child2Base && child2Base != nullptr) {
			const auto child2Ref = skyrim_cast<RE::BGSRefAlias*>(child2Base);

			if (child2Ref) {
				child2Actor = child2Ref->GetActorReference();
				if (child2Actor) {
					child2Near = Utility::GetPlayer()->GetCurrentLocation() == child2Actor->GetCurrentLocation();
				}
			}
		}

		if (adoptionQuest->IsRunning() && (child1Near || child2Near)) {
			return true;
		}

		return false;
	}

	static bool PlayerIsInHouseOrInn()
	{
		auto loc = Utility::GetPlayer()->GetCurrentLocation();
		auto util = Utility::GetSingleton();

		if (loc->HasKeyword(util->LocTypeInn) || loc->HasKeyword(util->LocTypePlayerHouse)) {
			return true;
		}
		return false;
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
