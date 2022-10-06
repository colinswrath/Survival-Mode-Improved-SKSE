#pragma once

#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "PlayerStatus.h"

class FormLoader
{
public:

	static FormLoader* GetSingleton()
	{
		static FormLoader formLoader;
		return &formLoader;
	}

	void LoadSettings();

	void LoadAllForms() 
	{
		if (!RE::TESDataHandler::GetSingleton()->LookupLoadedLightModByName("ccQDRSSE001-SurvivalMode.esl"))
			return;

		LoadHungerForms();
		LoadFatigueForms();
		//LoadColdForms();
		LoadMiscForms();

		logger::info("All forms are loaded.");
	}

	void LoadHungerForms()
	{	
		auto hungerSystem = NeedHunger::GetSingleton();

		hungerSystem->NeedPenaltyAV = RE::ActorValue::kVariable02;
		hungerSystem->ActorValPenaltyAttribute = RE::ActorValue::kStamina;
		hungerSystem->NeedPenaltyUIGlobal = RE::TESForm::LookupByEditorID("Survival_HungerAttributePenaltyPercent")->As<RE::TESGlobal>();

		hungerSystem->NeedSpell0 = RE::TESForm::LookupByEditorID("Survival_HungerStage0")->As<RE::SpellItem>();
		hungerSystem->NeedSpell1 = RE::TESForm::LookupByEditorID("Survival_HungerStage1")->As<RE::SpellItem>();
		hungerSystem->NeedSpell2 = RE::TESForm::LookupByEditorID("Survival_HungerStage2")->As<RE::SpellItem>();
		hungerSystem->NeedSpell3 = RE::TESForm::LookupByEditorID("Survival_HungerStage3")->As<RE::SpellItem>();
		hungerSystem->NeedSpell4 = RE::TESForm::LookupByEditorID("Survival_HungerStage4")->As<RE::SpellItem>();
		hungerSystem->NeedSpell5 = RE::TESForm::LookupByEditorID("Survival_HungerStage5")->As<RE::SpellItem>();

		hungerSystem->NeedStage1 = RE::TESForm::LookupByEditorID("Survival_HungerStage1Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage2 = RE::TESForm::LookupByEditorID("Survival_HungerStage2Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage3 = RE::TESForm::LookupByEditorID("Survival_HungerStage3Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage4 = RE::TESForm::LookupByEditorID("Survival_HungerStage4Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage5 = RE::TESForm::LookupByEditorID("Survival_HungerStage5Value")->As<RE::TESGlobal>();
		hungerSystem->NeedMaxValue = RE::TESForm::LookupByEditorID("Survival_HungerNeedMaxValue")->As<RE::TESGlobal>();

		hungerSystem->NeedMessage0 = RE::TESForm::LookupByEditorID("Survival_Hunger0Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage1 = RE::TESForm::LookupByEditorID("Survival_Hunger1Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage1Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger1MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage2 = RE::TESForm::LookupByEditorID("Survival_Hunger2Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage2Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger2MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage3 = RE::TESForm::LookupByEditorID("Survival_Hunger3Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage3Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger3MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage4 = RE::TESForm::LookupByEditorID("Survival_Hunger4Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage4Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger4MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage5 = RE::TESForm::LookupByEditorID("Survival_Hunger5Message")->As<RE::BGSMessage>();

		hungerSystem->NeedRate = RE::TESForm::LookupByEditorID("Survival_HungerNeedRate")->As<RE::TESGlobal>();		//TODO- Account for esp balance per /hr instead of /min
		hungerSystem->CurrentNeedStage = RE::TESForm::LookupByEditorID("SMI_CurrentHungerStage")->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedValue = RE::TESForm::LookupByEditorID("Survival_HungerNeedValue")->As<RE::TESGlobal>();

		hungerSystem->LastUpdateTimeStamp = RE::TESForm::LookupByEditorID("SMI_HungerLastUpdateTimeStamp")->As<RE::TESGlobal>();
		hungerSystem->NeedSleepRateMult = RE::TESForm::LookupByEditorID("Survival_NeedSleepReducedMetabolismMult")->As<RE::TESGlobal>();
		hungerSystem->NeedAttributePenaltyPercent = RE::TESForm::LookupByEditorID("Survival_HungerAttributePenaltyPercent")->As<RE::TESGlobal>();

		hungerSystem->Survival_FoodRestoreHungerVerySmall = RE::TESForm::LookupByEditorID("Survival_FoodRestoreHungerVerySmall")->As<RE::EffectSetting>();
		hungerSystem->Survival_FoodRestoreHungerSmall = RE::TESForm::LookupByEditorID("Survival_FoodRestoreHungerSmall")->As<RE::EffectSetting>();
		hungerSystem->Survival_FoodRestoreHungerMedium = RE::TESForm::LookupByEditorID("Survival_FoodRestoreHungerMedium")->As<RE::EffectSetting>();
		hungerSystem->Survival_FoodRestoreHungerLarge = RE::TESForm::LookupByEditorID("Survival_FoodRestoreHungerLarge")->As<RE::EffectSetting>();

		hungerSystem->Survival_HungerResistRacesMinor = RE::TESForm::LookupByEditorID("Survival_HungerResistRacesMinor")->As<RE::BGSListForm>();
		hungerSystem->Survival_RacialBonusMinor = RE::TESForm::LookupByEditorID("Survival_RacialBonusMinor")->As<RE::TESGlobal>();

		hungerSystem->Survival_HungerRestoreLargeAmount = RE::TESForm::LookupByEditorID("Survival_HungerRestoreLargeAmount")->As<RE::TESGlobal>();
		hungerSystem->Survival_HungerRestoreMediumAmount = RE::TESForm::LookupByEditorID("Survival_HungerRestoreMediumAmount")->As<RE::TESGlobal>();
		hungerSystem->Survival_HungerRestoreSmallAmount = RE::TESForm::LookupByEditorID("Survival_HungerRestoreSmallAmount")->As<RE::TESGlobal>();
		hungerSystem->Survival_HungerRestoreVerySmallAmount = RE::TESForm::LookupByEditorID("Survival_HungerRestoreVerySmallAmount")->As<RE::TESGlobal>();
		
	}

	void LoadFatigueForms()
	{
		auto fatigueSystem = NeedExhaustion::GetSingleton();

		fatigueSystem->NeedPenaltyAV = RE::ActorValue::kVariable03;
		fatigueSystem->ActorValPenaltyAttribute = RE::ActorValue::kMagicka;
		fatigueSystem->NeedPenaltyUIGlobal = RE::TESForm::LookupByEditorID("Survival_ExhaustionAttributePenaltyPercent")->As<RE::TESGlobal>();

		//Get these two by formID
		fatigueSystem->WellRested = RE::TESForm::LookupByID(RE::FormID(0x000FB984))->As<RE::SpellItem>();
		fatigueSystem->Rested = RE::TESForm::LookupByID(RE::FormID(0x000FB981))->As<RE::SpellItem>();

		fatigueSystem->NeedSpell1 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage1")->As<RE::SpellItem>();
		fatigueSystem->NeedSpell2 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage2")->As<RE::SpellItem>();
		fatigueSystem->NeedSpell3 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage3")->As<RE::SpellItem>();
		fatigueSystem->NeedSpell4 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage4")->As<RE::SpellItem>();
		fatigueSystem->NeedSpell5 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage5")->As<RE::SpellItem>();

		fatigueSystem->NeedStage1 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage1Value")->As<RE::TESGlobal>();
		fatigueSystem->NeedStage2 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage2Value")->As<RE::TESGlobal>();
		fatigueSystem->NeedStage3 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage3Value")->As<RE::TESGlobal>();
		fatigueSystem->NeedStage4 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage4Value")->As<RE::TESGlobal>();
		fatigueSystem->NeedStage5 = RE::TESForm::LookupByEditorID("Survival_ExhaustionStage5Value")->As<RE::TESGlobal>();
		fatigueSystem->NeedMaxValue = RE::TESForm::LookupByEditorID("Survival_ExhaustionNeedMaxValue")->As<RE::TESGlobal>();

		fatigueSystem->WellRestedMessage = RE::TESForm::LookupByEditorID("WellRestedMessage")->As<RE::BGSMessage>();
		fatigueSystem->RestedMessage = RE::TESForm::LookupByEditorID("RestedMessage")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage1 = RE::TESForm::LookupByEditorID("Survival_Exhaustion1Message")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage1Decreasing = RE::TESForm::LookupByEditorID("Survival_Exhaustion1MessageDecreasing")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage2 = RE::TESForm::LookupByEditorID("Survival_Exhaustion2Message")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage2Decreasing = RE::TESForm::LookupByEditorID("Survival_Exhaustion2MessageDecreasing")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage3 = RE::TESForm::LookupByEditorID("Survival_Exhaustion3Message")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage3Decreasing = RE::TESForm::LookupByEditorID("Survival_Exhaustion3MessageDecreasing")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage4 = RE::TESForm::LookupByEditorID("Survival_Exhaustion4Message")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage4Decreasing = RE::TESForm::LookupByEditorID("Survival_Exhaustion4MessageDecreasing")->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage5 = RE::TESForm::LookupByEditorID("Survival_Exhaustion5Message")->As<RE::BGSMessage>();

		fatigueSystem->NeedRate = RE::TESForm::LookupByEditorID("Survival_ExhaustionNeedRate")->As<RE::TESGlobal>();
		fatigueSystem->CurrentNeedStage = RE::TESForm::LookupByEditorID("SMI_CurrentExhaustionStage")->As<RE::TESGlobal>();
		fatigueSystem->CurrentNeedValue = RE::TESForm::LookupByEditorID("Survival_ExhaustionNeedValue")->As<RE::TESGlobal>();

		fatigueSystem->LastUpdateTimeStamp = RE::TESForm::LookupByEditorID("SMI_ExhaustionLastUpdateTimeStamp")->As<RE::TESGlobal>();
		fatigueSystem->NeedSleepRateMult = RE::TESForm::LookupByEditorID("Survival_NeedSleepReducedMetabolismMult")->As<RE::TESGlobal>();
		fatigueSystem->NeedAttributePenaltyPercent = RE::TESForm::LookupByEditorID("Survival_ExhaustionAttributePenaltyPercent")->As<RE::TESGlobal>();

		fatigueSystem->PlayerSleepQuest = RE::TESForm::LookupByEditorID("PlayerSleepQuest")->As<RE::TESQuest>();
		fatigueSystem->Survival_ExhaustionRestorePerHour = RE::TESForm::LookupByEditorID("Survival_ExhaustionRestorePerHour")->As<RE::TESGlobal>();

		fatigueSystem->Survival_ExhaustionResistRacesMajor = RE::TESForm::LookupByEditorID("Survival_ExhaustionResistRacesMajor")->As<RE::BGSListForm>();
		fatigueSystem->Survival_ExhaustionResistRacesMinor = RE::TESForm::LookupByEditorID("Survival_ExhaustionResistRacesMinor")->As<RE::BGSListForm>();
		fatigueSystem->Survival_RacialBonusMinor = RE::TESForm::LookupByEditorID("Survival_RacialBonusMinor")->As<RE::TESGlobal>();
		fatigueSystem->Survival_RacialBonusMajor = RE::TESForm::LookupByEditorID("Survival_RacialBonusMajor")->As<RE::TESGlobal>();

		//VampireBloodMessage
	}

	void LoadColdForms()
	{
		//Cold penalty AV is 4
	}

	void LoadMiscForms()
	{
		auto playerStatus = PlayerStatus::GetSingleton();
		playerStatus->Survival_ModeToggle = RE::TESForm::LookupByEditorID("Survival_ModeToggle")->As<RE::TESGlobal>();
		playerStatus->Survival_ModeEnabled = RE::TESForm::LookupByEditorID("Survival_ModeEnabled")->As<RE::TESGlobal>();
		playerStatus->Survival_ModeEnabledShared = RE::TESForm::LookupByEditorID("Survival_ModeEnabledShared")->As<RE::TESGlobal>();
		playerStatus->Survival_ModeCanBeEnabled = RE::TESForm::LookupByEditorID("Survival_ModeCanBeEnabled")->As<RE::TESGlobal>();

		playerStatus->Survival_OblivionAreaMessage = RE::TESForm::LookupByEditorID("Survival_OblivionAreaMessage")->As<RE::BGSMessage>();
		playerStatus->Survival_OblivionCells = RE::TESForm::LookupByEditorID("Survival_OblivionCells")->As<RE::BGSListForm>();
		playerStatus->Survival_OblivionLocations = RE::TESForm::LookupByEditorID("Survival_OblivionLocations")->As<RE::BGSListForm>();
		playerStatus->Survival_OblivionAreas = RE::TESForm::LookupByEditorID("Survival_OblivionAreas")->As<RE::BGSListForm>();
		
		RE::SpellItem* isVampireSpell = RE::TESForm::LookupByEditorID("SMI_VampireSpell")->As<RE::SpellItem>();
		playerStatus->IsVampireConditions = &isVampireSpell->effects[0]->conditions;

		playerStatus->DA16 = RE::TESForm::LookupByEditorID("DA16")->As<RE::TESQuest>();

	}
};
