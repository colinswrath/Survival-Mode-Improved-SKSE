#pragma once

//TODO - This file needs a major rework/split
enum class AREA_TYPE
{
	kAreaTypeChillyInterior = -1,
	kAreaTypeInterior = 0,
	kAreaTypeWarm = 1,
	kAreaTypeCool = 2,
	kAreaTypeFreezing = 3,
	kAreaTypeReach = 4
};

namespace HashUtility
{
    static constexpr uint32_t hash(const char* data, const size_t size) noexcept
    {
        uint32_t hash = 5381;

        for (const char* c = data; c < data + size; ++c) {
            hash = ((hash << 5) + hash) + (unsigned char)*c;
        }

        return hash;
    }

    constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
    {
        return hash(str, size);
    }

}

class ModVersion
{
public:
    int Major;
    int Minor;
    int Patch;

    ModVersion(int major = 0, int minor = 0, int patch = 0) : Major(major), Minor(minor), Patch(patch) {}

    ModVersion(const std::vector<std::string>& versionVec)
    {
        if (versionVec.size() != 3) {
            logger::info("Invalid vector version size (needs to be 3). Defaulting to 0.0.0");
            Major = 0;
            Minor = 0;
            Patch = 0;
            return;
        }
        Major = std::stoi(versionVec[0]);
        Minor = std::stoi(versionVec[1]);
        Patch = std::stoi(versionVec[2]);
    }

    ModVersion& operator=(const ModVersion& other)
    {
        if (this != &other) {
            Major = other.Major;
            Minor = other.Minor;
            Patch = other.Patch;
        }
        return *this;
    }

    bool operator>(const ModVersion& other) const
    {
        if (Major > other.Major)
            return true;
        if (Major < other.Major)
            return false;
        if (Minor > other.Minor)
            return true;
        if (Minor < other.Minor)
            return false;
        return Patch > other.Patch;
    }

    bool operator<(const ModVersion& other) const
    {
        if (Major < other.Major)
            return true;
        if (Major > other.Major)
            return false;
        if (Minor < other.Minor)
            return true;
        if (Minor > other.Minor)
            return false;
        return Patch < other.Patch;
    }

    bool operator>=(const ModVersion& other) const { return (*this > other) || (*this == other); }

    bool operator<=(const ModVersion& other) const { return (*this < other) || (*this == other); }

    bool operator==(const ModVersion& other) const { return Major == other.Major && Minor == other.Minor && Patch == other.Patch; }

    std::string getVersionAsString() const
    {
        std::ostringstream oss;
        oss << Major << '.' << Minor << '.' << Patch;
        return oss.str();
    }

    std::vector<int> getVersionAsVector() const { return { Major, Minor, Patch }; }

    bool IsEmptyVersion() const
    {
        if (Major == 0 && Minor == 0 && Patch == 0) {
            return true;
        }
        else {
            return false;
        }
    }
};

class Utility
{
public:
	RE::TESGlobal* Survival_ModeToggle;
	RE::TESGlobal* Survival_ModeEnabled;
	RE::TESGlobal* Survival_ModeEnabledShared;
	RE::TESGlobal* Survival_ModeCanBeEnabled;

	RE::TESGlobal* SMI_HungerShouldBeEnabled;
	RE::TESGlobal* SMI_ColdShouldBeEnabled;
	RE::TESGlobal* SMI_ExhaustionShouldBeEnabled;

	RE::SpellItem* Survival_abLowerCarryWeightSpell;
	RE::SpellItem* Survival_abLowerRegenSpell;
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

	RE::SpellItem* BnBInjury1;
    RE::SpellItem* BnBInjury2;
    RE::SpellItem* BnBInjury3;

    RE::TESGlobal* MAG_InjuriesSMOnly;
    RE::TESGlobal* MAG_InjuriesAndRest;

	RE::SpellItem* Survival_DiseaseBrownRot;
	RE::SpellItem* Survival_DiseaseGreenspore;
	
	RE::SpellItem* Survival_DiseaseGutworm;
	RE::SpellItem* Survival_DiseaseGutworm2;
	RE::SpellItem* Survival_DiseaseGutworm3;

	RE::BGSListForm* Survival_BrownRotCarryingRaces;
	RE::BGSListForm* Survival_GreensporeCarryingRaces;
	RE::BGSListForm* Survival_GutwormCarryingRaces;

	RE::BGSListForm* SMI_NoNeedsRaces;

	RE::SpellItem* Survival_OblivionDisplaySpell;

	RE::BGSListForm* Survival_OblivionCells;
	RE::BGSListForm* Survival_OblivionLocations;
	RE::BGSListForm* Survival_OblivionAreas;

	RE::BGSListForm* Survival_InteriorAreas;
	RE::BGSListForm* Survival_ColdInteriorLocations;
	RE::BGSListForm* Survival_ColdInteriorCells;
	RE::BGSListForm* Survival_SurvivalDiseases;
	RE::BGSListForm* SMI_WellRestedObjectsList;

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

	RE::TESConditionItem* BrumaIsInFreezingArea;
	RE::TESConditionItem* BrumaIsComfortableArea;
	RE::TESConditionItem* BrumaIsCityArea;

	RE::EffectSetting* WerewolfFeedRestoreHealth;
	RE::EffectSetting* DA11AbFortifyHealth;
    RE::EffectSetting* Survival_FireCloakFreezingWaterDesc;
    RE::EffectSetting* Survival_FoodRestoreHungerLargeVampire;

	RE::TESCondition* IsVampireConditions;
	RE::TESCondition* IsWerewolfConditions;
	RE::TESConditionItem* IsInJailCondition;

	RE::TESConditionItem* AdoptionHomeLocationCond;

	RE::BGSMessage* Survival_OblivionAreaMessage;
	RE::BGSMessage* Survival_HelpSurvivalModeLong;
	RE::BGSMessage* Survival_HelpSurvivalModeLongXbox;

	RE::BGSPerk* Survival_TempleBlessingCostPerk;
	RE::BGSPerk* Undeath_LichPerk;

	RE::TESQuest* DA16;
	RE::TESQuest* MQ101;
	RE::TESQuest* RelationshipMarriageFIN;
	RE::TESQuest* UnboundQuest;
	RE::TESQuest* BYOHRelationshipAdoption;

    std::vector<RE::TESQuest*> smQuestsToHandle;

    RE::TESRegion* WeatherMountains;
    RE::TESRegion* WeatherSnow;
    RE::TESRegion* WeatherReach;

	RE::TESRegion* CYRWeatherBruma;
	RE::TESRegion* CYRWeatherGreatForestNorth;
	RE::TESRegion* CYRWeatherMountainsSnow;

	RE::TESRegion* WyrmstoothSteampools;
	RE::TESRegion* WyrmstoothForest;
	RE::TESRegion* WyrmstoothMarsh;

	RE::TESWorldSpace* WyrmstoothWorldspace;
	
	RE::BSFixedString hungerVerySmallDesc;
	RE::BSFixedString hungerSmallDesc;
	RE::BSFixedString hungerMediumDesc;
	RE::BSFixedString hungerLargeDesc;

	RE::BGSKeyword* LocTypeInn;
	RE::BGSKeyword* LocTypePlayerHouse;

	uintptr_t PlayerSingletonAddress;
	uintptr_t UISingletonAddress;
	uintptr_t CalendarSingletonAddress;
	uintptr_t MenuControlsSingletonAddress;
	uintptr_t GetWarmthRatingAddress;
	uintptr_t DoCombatSpellApplyAddress;
	uintptr_t EnableFtAddress;
	uintptr_t IsFtEnabledAddress;

    ModVersion starfrostVer;

	bool WasInOblivion = false;
	bool DisableFastTravel = true;
	bool AutoStart = true;
	bool DisableCarryWeightPenalty = false;
	bool DisableDiseaseApplicator = false;
	bool starfrostInstalled = false;
	bool forceEnableFoodPoisoning = false;
	bool forceUpdateGlobalValues = false;

	bool vampireHunger = true;
	bool vampireCold = true;
	bool vampireExhaustion = true;

    bool  handleInjuries      = false;

	float MaxAvPenaltyPercent = 1.0f;

    float MainUpdateInterval = 1.0f;
    float AvUpdateInterval   = 0.5f;

	//Global Load overwrite variables
	float LoadColdStage1Val = 0.0f;
	float LoadColdStage2Val = 0.0f;
	float LoadColdStage3Val = 0.0f;
	float LoadColdStage4Val = 0.0f;
	float LoadColdStage5Val = 0.0f;

	float LoadExhaustionStage1Val = 0.0f;
	float LoadExhaustionStage2Val = 0.0f;
	float LoadExhaustionStage3Val = 0.0f;
	float LoadExhaustionStage4Val = 0.0f;
	float LoadExhaustionStage5Val = 0.0f;

	float LoadHungerStage1Val = 0.0f;
	float LoadHungerStage2Val = 0.0f;
	float LoadHungerStage3Val = 0.0f;
	float LoadHungerStage4Val = 0.0f;
	float LoadHungerStage5Val = 0.0f;

	float coldShouldBeEnabled = 0.0f;
	float exhaustionShouldBeEnabled = 0.0f;
	float hungerShouldBeEnabled = 0.0f;

	float coldAVPenDisabled = 0.0f;
	float exhaustionAVPenDisabled = 0.0f;
	float hungerAVPenDisabled = 0.0f;

	float coldResistMaxValue = 0.0f;

	float coldMaxValue = 0.0f;
	float exhaustionMaxValue = 0.0f;
	float hungerMaxValue = 0.0f;

	float coldAfflictionChance = 0.0f;
	float hungerAfflictionChance = 0.0f;
	float exhaustionAfflictionChance = 0.0f;

    float injury1AVPercent = 0.0f;
    float injury2AVPercent = 0.0f;
    float injury3AVPercent = 0.0f;

	float exhaustionRestorePerHour = 0.0f;

	float coldRate = 0.0f;
	float hungerRate = 0.0f;
	float exhaustionRate = 0.0f;

	static Utility* GetSingleton()
	{
		static Utility playerStatus;
		return &playerStatus;
	}

	AREA_TYPE GetCurrentAreaType()
	{
		auto player = Utility::GetPlayer();
		auto playerParentCell = player->GetParentCell();
		auto worldspace = player->GetWorldspace();
		RE::ConditionCheckParams playerParam(player, nullptr);

		if ((playerParentCell && playerParentCell->IsInteriorCell()) || (worldspace && Survival_InteriorAreas->HasForm(worldspace))) {
			if (playerParentCell && (Survival_ColdInteriorLocations->HasForm(playerParentCell) || 
				Survival_ColdInteriorCells->HasForm(playerParentCell))) {
				return AREA_TYPE::kAreaTypeChillyInterior;
			} else {
				return AREA_TYPE::kAreaTypeInterior;
			}
		} else if (IsInPineForestFreezingArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeFreezing;
		} else if (IsInFallForestFreezingArea->IsTrue(player, nullptr)) {
			return AREA_TYPE::kAreaTypeFreezing;
		} else if (IsInWarmArea->IsTrue(player, nullptr) || 
			(WTIsInWarmArea && WTIsInWarmArea->IsTrue(player, nullptr))) {
			
			return AREA_TYPE::kAreaTypeWarm;

		} else if (IsInCoolArea->IsTrue(player, nullptr) || 
			(WTIsInCoolArea && WTIsInCoolArea->IsTrue(player, nullptr)) || 
			(BrumaIsCityArea && BrumaIsCityArea->IsTrue(playerParam))) {

			return AREA_TYPE::kAreaTypeCool;

		} else if (IsInFreezingArea->IsTrue(player, nullptr) || 
			(WTIsInFreezingArea && WTIsInFreezingArea->IsTrue(player, nullptr)) || 
			(BrumaIsInFreezingArea && BrumaIsInFreezingArea->IsTrue(playerParam))) {

			return AREA_TYPE::kAreaTypeFreezing;

		} else if (IsInReachArea && IsInReachArea->IsTrue(player, nullptr)) {
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
        return clib_util::RNG().Generate<float>(min, max);
	}

	static bool IsSurvivalEnabled()
	{
		auto util = Utility::GetSingleton();
		return util->Survival_ModeEnabled->value;
	}

	static void ShowNotification(RE::BGSMessage* msg, bool messageBox=false)
	{
		RE::BSString messageDesc;
		msg->GetDescription(messageDesc, msg);
		if (messageBox) {
			RE::DebugMessageBox(messageDesc.c_str());
			/*
			auto tut = RE::UI::GetSingleton()->GetMenu(RE::TutorialMenu::MENU_NAME);
			RE::UIMessage 
			tut->ProcessMessage()
			auto uiQueue = RE::UIMessageQueue::GetSingleton();
			uiQueue->AddMessage(RE::TutorialMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, uiQueue->CreateUIMessageData("TEST")); 
			*/
		} else {
			RE::DebugNotification(messageDesc.c_str());
		}
	}

    void ClearSurvivalModeQuestScripts()
    {
        logger::info("Searching for attached sm quest scripts");

        for(auto* quest: smQuestsToHandle)
        {
            DetachQuestScripts(quest);
        }
    }

    static void DetachQuestScripts(RE::TESQuest* quest)
    {
        auto       vm     = RE::BSScript::Internal::VirtualMachine::GetSingleton();
        const auto policy = vm ? vm->GetObjectHandlePolicy() : nullptr;

        const auto          handle = policy->GetHandleForObject(quest->GetFormType(), quest);

        RE::BSSpinLockGuard locker(vm->attachedScriptsLock);
        if (const auto it = vm->attachedScripts.find(handle); it != vm->attachedScripts.end()) {
            for (auto& script : it->second) {
                if (auto typeInfo = script ? script->GetTypeInfo() : nullptr) {
                    logger::info("Attached script found. Clearing {}", typeInfo->name.c_str());
                }
            }
        }

        vm->ResetAllBoundObjects(handle);
        vm->GetObjectBindPolicy()->bindInterface->RemoveAllBoundObjects(handle);
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
				if ((setting->data.archetype == RE::EffectSetting::Archetype::kCloak && setting->data.resistVariable == RE::ActorValue::kResistFire) || setting == Survival_FireCloakFreezingWaterDesc) {
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
		const auto* player = GetPlayer();
		const auto* location = player->GetCurrentLocation();
		const auto* worldspace = player->GetWorldspace();
		const auto* cell = player->GetParentCell();

		auto da16Stage = DA16->GetCurrentStageID();
		if ((location && Survival_OblivionLocations->HasForm(location)) ||
			(worldspace && Survival_OblivionAreas->HasForm(worldspace)) ||
			(cell && Survival_OblivionCells->HasForm(cell)) ||
			(da16Stage >= 145 && da16Stage < 160)) {
			return true;
		}

		return false;
	}

	static void RemoveSurvivalDiseases()
	{
		auto util = Utility::GetSingleton();
		util->Survival_SurvivalDiseases->ForEachForm([&](RE::TESForm* a_form) {
			if (auto disease = a_form->As<RE::SpellItem>()) {
				Utility::GetPlayer()->RemoveSpell(disease);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});
	}

	static bool DisableFTCheck()
	{
		if(!Utility::IsOnFlyingMount(Utility::GetPlayer()))
		{
			auto enabled = Utility::GetSingleton()->Survival_ModeEnabled->value;
			if (enabled && enabled == 1.0f) {
				return true;
			}
		}

		return false;
	}

	static bool PlayerCanGetWellRested()
	{
		return !PlayerIsWerewolf();
	}

	static bool PlayerIsVampire()
	{
		auto util = Utility::GetSingleton();
		return util->IsVampireConditions->IsTrue(GetPlayer(), nullptr);
	}

    static bool VampireFeedCheck()
    {
        auto* package = Utility::GetPlayer()->GetCurrentPackage();

        if (package && package->packData.packType == RE::PACKAGE_PROCEDURE_TYPE::kVampireFeed) {
            return true;
        }
        return false;
    }

	static bool PlayerIsNoNeedsRace()
	{
		bool lich = false;
		auto util = Utility::GetSingleton();
		if (util->Undeath_LichPerk && GetPlayer()->HasPerk(util->Undeath_LichPerk)) {
			lich = true;
		} else if (util->SMI_NoNeedsRaces && util->SMI_NoNeedsRaces->HasForm(GetPlayer()->GetRace())) {
			lich = true;
		} 

		return lich;
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

		auto& aliases = relMarriageQuest->aliases;
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

		if (relMarriageQuest->IsRunning() && relMarriageQuest->currentStage >= 10) {
			auto playerLoc = Utility::GetPlayer()->GetCurrentLocation();
			auto interest = loveInterestRef->GetActorReference();
			auto interestLoc = interest == nullptr ? nullptr : interest->GetCurrentLocation();

			if ((playerLoc && interestLoc) && playerLoc == interestLoc)
			{
				return true;
			}
		}

		return false;
	}

	static bool PlayerIsNearAdopted()
	{
		auto util = Utility::GetSingleton();
		auto adoptionQuest = util->BYOHRelationshipAdoption;

		auto child1String = RE::BSFixedString("Child1");
		auto child2String = RE::BSFixedString("Child2");

		auto& aliases = adoptionQuest->aliases;

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

		if (loc && (loc->HasKeyword(util->LocTypeInn) || loc->HasKeyword(util->LocTypePlayerHouse))) {
			return true;
		}

		return false;
	}

	static bool PlayerIsNearWellRestedBed()
	{
		auto player = Utility::GetPlayer();
		auto util = Utility::GetSingleton();

		bool nearWellRested = false;
		RE::TES::GetSingleton()->ForEachReferenceInRange(player, 300.0f, [&](RE::TESObjectREFR* b_ref) {
			if (b_ref && !b_ref->IsDisabled() && b_ref->Is3DLoaded()) {
				if (const auto base = b_ref->GetBaseObject(); base) {
                    if (base) {
					    if (util->SMI_WellRestedObjectsList->HasForm(base)) {
						    nearWellRested = true;
						    return RE::BSContainer::ForEachResult::kStop;
					    }
                    }
				}
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});
		
		return nearWellRested;
	}

	//CREDIT-> po3 in papyrus extender
	static std::string GetFormEditorID(const RE::TESForm* a_form)
	{
		switch (a_form->GetFormType()) {
		case RE::FormType::Keyword:
		case RE::FormType::LocationRefType:
		case RE::FormType::Action:
		case RE::FormType::MenuIcon:
		case RE::FormType::Global:
		case RE::FormType::HeadPart:
		case RE::FormType::Race:
		case RE::FormType::Sound:
		case RE::FormType::Script:
		case RE::FormType::Navigation:
		case RE::FormType::Cell:
		case RE::FormType::WorldSpace:
		case RE::FormType::Land:
		case RE::FormType::NavMesh:
		case RE::FormType::Dialogue:
		case RE::FormType::Quest:
		case RE::FormType::Idle:
		case RE::FormType::AnimatedObject:
		case RE::FormType::ImageAdapter:
		case RE::FormType::VoiceType:
		case RE::FormType::Ragdoll:
		case RE::FormType::DefaultObject:
		case RE::FormType::MusicType:
		case RE::FormType::StoryManagerBranchNode:
		case RE::FormType::StoryManagerQuestNode:
		case RE::FormType::StoryManagerEventNode:
		case RE::FormType::SoundRecord:
			return a_form->GetFormEditorID();
		default:
			{
				static auto tweaks = GetModuleHandle(L"po3_Tweaks");
				if (tweaks) {
					static auto func = reinterpret_cast<const char* (*)(std::uint32_t)>(GetProcAddress(tweaks, "GetFormEditorID"));
					if (func) {
						return func(a_form->formID);
					}
				}
				return {};
			}
		}
	}

	static bool string_Contains(std::string mainString, std::string subString)
	{
		std::transform(mainString.begin(), mainString.end(), mainString.begin(), [](unsigned char c) { return static_cast<unsigned char> (std::tolower(c)); });
		std::transform(subString.begin(), subString.end(), subString.begin(), [](unsigned char c) { return static_cast <unsigned char>(std::tolower(c)); });
		return mainString.find(subString) != std::string::npos;
	}

	static bool PlayerIsBeastFormRace()
	{
		auto menuControls = Utility::GetSingleton()->GetMenuControls();
		return menuControls->InBeastForm();
	}

	static bool IsPlayerInDialogue()
	{
		return Utility::GetSingleton()->GetUI()->IsMenuOpen(RE::DialogueMenu::MENU_NAME);
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

	static void DoCombatSpellApply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target)
	{
		using func_t = decltype(&Utility::DoCombatSpellApply);
		REL::Relocation<func_t> func{ Utility::GetSingleton()->DoCombatSpellApplyAddress };
		return func(actor, spell, target);
	}

	static void EnableFastTravel(void* a1, void* a2, void* a3, bool a_enable)
	{
		using func_t = decltype(&Utility::EnableFastTravel);
		REL::Relocation<func_t> func{ Utility::GetSingleton()->EnableFtAddress };
        return func(a1, a2, a3, a_enable);
	}

	static bool IsFastTravelEnabled()
	{
		using func_t = decltype(&Utility::IsFastTravelEnabled);
		REL::Relocation<func_t> func{ Utility::GetSingleton()->IsFtEnabledAddress };
		return func();
	}

    static RE::TESFile* LookupLoadedModByName(std::string_view a_modName)
    {
        for (auto& file : RE::TESDataHandler::GetSingleton()->compiledFileCollection.files) {
            if (a_modName.size() == strlen(file->fileName) && _strnicmp(file->fileName, a_modName.data(), a_modName.size()) == 0) {
                return file;
            }
        }
        return nullptr;
    }

    static const RE::TESFile* LookupLoadedLightModByName(std::string_view a_modName)
    {
        for (auto& smallFile : RE::TESDataHandler::GetSingleton()->compiledFileCollection.smallFiles) {
            if (a_modName.size() == strlen(smallFile->fileName) && _strnicmp(smallFile->fileName, a_modName.data(), a_modName.size()) == 0) {
                return smallFile;
            }
        }
        return nullptr;
    }

    static std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string              token;
        std::istringstream       tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static std::vector<std::string> TokenizeString(const std::string& s)
    {
        std::vector<std::string> tokens;
        tokens.reserve(s.size());

        for (char c : s) {
            tokens.emplace_back(1, c);
        }

        return tokens;
    }

    static std::vector<std::string> ParseVersionString(const std::string& str)
    {
        std::vector<std::string> version;
        version = split(str, '.');

        if (version.size() == 2) {
            return TokenizeString(version[0]);
        }
        else if (version.size() != 3) {
            logger::error("Unable to parse MAJOR.MINOR.PATCH from {}", str);
        }
        return version;
    }
};
