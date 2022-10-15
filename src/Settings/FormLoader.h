#pragma once

#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "Needs/NeedCold.h"
#include "Utility.h"

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
		logger::info("Loading all forms.");

		if (!RE::TESDataHandler::GetSingleton()->LookupLoadedLightModByName("ccQDRSSE001-SurvivalMode.esl"))
			return;

		LoadHungerForms();
		logger::info("Hunger forms are loaded.");
		LoadFatigueForms();
		logger::info("Fatigue forms are loaded.");
		LoadColdForms();
		logger::info("Cold forms are loaded.");
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

		hungerSystem->Survival_FoodRawMeat = RE::TESForm::LookupByEditorID("Survival_FoodRawMeat")->As<RE::BGSListForm>();
		hungerSystem->VendorItemFoodRaw = RE::TESForm::LookupByEditorID("VendorItemFoodRaw")->As<RE::BGSKeyword>();
		hungerSystem->Survival_FoodPoisoningImmuneRaces = RE::TESForm::LookupByEditorID("Survival_FoodPoisoningImmuneRaces")->As<RE::BGSListForm>();
		hungerSystem->Survival_DiseaseFoodPoisoningKeyword = RE::TESForm::LookupByEditorID("Survival_DiseaseFoodPoisoningKeyword")->As<RE::BGSKeyword>();
		hungerSystem->Survival_FoodPoisoningMsg = RE::TESForm::LookupByEditorID("Survival_FoodPoisoningMsg")->As<RE::BGSMessage>();
		hungerSystem->Survival_DiseaseFoodPoisoning = RE::TESForm::LookupByEditorID("Survival_DiseaseFoodPoisoning")->As<RE::SpellItem>();
		
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

		fatigueSystem->Survival_ExhaustionOverEncumberedMult = RE::TESForm::LookupByEditorID("Survival_ExhaustionOverEncumberedMult")->As<RE::TESGlobal>();

		//VampireBloodMessage
	}

	void LoadColdForms()
	{
		//Cold penalty AV is 4

		auto coldSystem = NeedCold::GetSingleton();

		coldSystem->NeedPenaltyAV = RE::ActorValue::kVariable04;
		coldSystem->ActorValPenaltyAttribute = RE::ActorValue::kHealth;
		coldSystem->NeedPenaltyUIGlobal = RE::TESForm::LookupByEditorID("Survival_ColdAttributePenaltyPercent")->As<RE::TESGlobal>();

		coldSystem->Survival_ColdTargetGameHoursToNumb = RE::TESForm::LookupByEditorID("Survival_ColdTargetGameHoursToNumb")->As<RE::TESGlobal>();
		coldSystem->SMI_ColdRate = RE::TESForm::LookupByEditorID("SMI_ColdRate")->As<RE::TESGlobal>();
		coldSystem->SMI_CurrentAmbientTemp = RE::TESForm::LookupByEditorID("SMI_CurrentAmbientTemp")->As<RE::TESGlobal>();

		coldSystem->NeedSpell0 = RE::TESForm::LookupByEditorID("Survival_ColdStage0")->As<RE::SpellItem>();
		coldSystem->NeedSpell1 = RE::TESForm::LookupByEditorID("Survival_ColdStage1")->As<RE::SpellItem>();
		coldSystem->NeedSpell2 = RE::TESForm::LookupByEditorID("Survival_ColdStage2")->As<RE::SpellItem>();
		coldSystem->NeedSpell3 = RE::TESForm::LookupByEditorID("Survival_ColdStage3")->As<RE::SpellItem>();
		coldSystem->NeedSpell4 = RE::TESForm::LookupByEditorID("Survival_ColdStage4")->As<RE::SpellItem>();
		coldSystem->NeedSpell5 = RE::TESForm::LookupByEditorID("Survival_ColdStage5")->As<RE::SpellItem>();

		coldSystem->Survival_FreezingWaterDamage = RE::TESForm::LookupByEditorID("Survival_FreezingWaterDamage")->As<RE::SpellItem>();

		coldSystem->NeedStage1 = RE::TESForm::LookupByEditorID("Survival_ColdStage1Value")->As<RE::TESGlobal>();
		coldSystem->NeedStage2 = RE::TESForm::LookupByEditorID("Survival_ColdStage2Value")->As<RE::TESGlobal>();
		coldSystem->NeedStage3 = RE::TESForm::LookupByEditorID("Survival_ColdStage3Value")->As<RE::TESGlobal>();
		coldSystem->NeedStage4 = RE::TESForm::LookupByEditorID("Survival_ColdStage4Value")->As<RE::TESGlobal>();
		coldSystem->NeedStage5 = RE::TESForm::LookupByEditorID("Survival_ColdStage5Value")->As<RE::TESGlobal>();
		coldSystem->NeedMaxValue = RE::TESForm::LookupByEditorID("Survival_ColdNeedMaxValue")->As<RE::TESGlobal>();

		coldSystem->Survival_ColdConditionStage0 = RE::TESForm::LookupByEditorID("Survival_ColdConditionStage0")->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage1 = RE::TESForm::LookupByEditorID("Survival_ColdConditionStage1")->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage2 = RE::TESForm::LookupByEditorID("Survival_ColdConditionStage2")->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage3 = RE::TESForm::LookupByEditorID("Survival_ColdConditionStage3")->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage4 = RE::TESForm::LookupByEditorID("Survival_ColdConditionStage4")->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage5 = RE::TESForm::LookupByEditorID("Survival_ColdConditionStage5")->As<RE::BGSMessage>();

		coldSystem->NeedMessage0 = RE::TESForm::LookupByEditorID("Survival_Cold0Message")->As<RE::BGSMessage>();
		coldSystem->NeedMessage1 = RE::TESForm::LookupByEditorID("Survival_Cold1Message")->As<RE::BGSMessage>();
		coldSystem->NeedMessage1Decreasing = RE::TESForm::LookupByEditorID("Survival_Cold1MessageDecreasing")->As<RE::BGSMessage>();
		coldSystem->NeedMessage2 = RE::TESForm::LookupByEditorID("Survival_Cold2Message")->As<RE::BGSMessage>();
		coldSystem->NeedMessage2Decreasing = RE::TESForm::LookupByEditorID("Survival_Cold2MessageDecreasing")->As<RE::BGSMessage>();
		coldSystem->NeedMessage3 = RE::TESForm::LookupByEditorID("Survival_Cold3Message")->As<RE::BGSMessage>();
		coldSystem->NeedMessage3Decreasing = RE::TESForm::LookupByEditorID("Survival_Cold3MessageDecreasing")->As<RE::BGSMessage>();
		coldSystem->NeedMessage4 = RE::TESForm::LookupByEditorID("Survival_Cold4Message")->As<RE::BGSMessage>();
		coldSystem->NeedMessage4Decreasing = RE::TESForm::LookupByEditorID("Survival_Cold4MessageDecreasing")->As<RE::BGSMessage>();
		coldSystem->NeedMessage5 = RE::TESForm::LookupByEditorID("Survival_Cold5Message")->As<RE::BGSMessage>();

		coldSystem->Survival_WaterFreezingMessage = RE::TESForm::LookupByEditorID("Survival_WaterFreezingMessage")->As<RE::BGSMessage>();

		coldSystem->CurrentNeedStage = RE::TESForm::LookupByEditorID("SMI_CurrentColdStage")->As<RE::TESGlobal>();
		coldSystem->CurrentNeedValue = RE::TESForm::LookupByEditorID("Survival_ColdNeedValue")->As<RE::TESGlobal>();
		coldSystem->Survival_ColdLevelInFreezingWater = RE::TESForm::LookupByEditorID("Survival_ColdLevelInFreezingWater")->As<RE::TESGlobal>();

		coldSystem->Survival_AshWeather = RE::TESForm::LookupByEditorID("Survival_AshWeather")->As<RE::BGSListForm>();
		coldSystem->Survival_BlizzardWeather = RE::TESForm::LookupByEditorID("Survival_BlizzardWeather")->As<RE::BGSListForm>();
		coldSystem->SMI_ColdCloudyWeather = RE::TESForm::LookupByEditorID("SMI_ColdCloudyWeather")->As<RE::BGSListForm>();
		coldSystem->Survival_WarmUpObjectsList = RE::TESForm::LookupByEditorID("Survival_WarmUpObjectsList")->As<RE::BGSListForm>();	

		coldSystem->LastUpdateTimeStamp = RE::TESForm::LookupByEditorID("SMI_ColdLastUpdateTimeStamp")->As<RE::TESGlobal>();
		coldSystem->NeedSleepRateMult = RE::TESForm::LookupByEditorID("Survival_NeedSleepReducedMetabolismMult")->As<RE::TESGlobal>();
		coldSystem->NeedAttributePenaltyPercent = RE::TESForm::LookupByEditorID("Survival_ColdAttributePenaltyPercent")->As<RE::TESGlobal>();
		coldSystem->Survival_ColdResistMaxValue = RE::TESForm::LookupByEditorID("Survival_ColdResistMaxValue")->As<RE::TESGlobal>();
		coldSystem->Survival_TemperatureLevel = RE::TESForm::LookupByEditorID("Survival_TemperatureLevel")->As<RE::TESGlobal>();
		coldSystem->Survival_ColdLevelInFreezingWater = RE::TESForm::LookupByEditorID("Survival_ColdLevelInFreezingWater")->As<RE::TESGlobal>();
		coldSystem->Survival_LastWaterFreezingMsgTime = RE::TESForm::LookupByEditorID("Survival_LastWaterFreezingMsgTime")->As<RE::TESGlobal>();
		coldSystem->Survival_ColdRestoreSmallAmount = RE::TESForm::LookupByEditorID("Survival_ColdRestoreSmallAmount")->As<RE::TESGlobal>();

		coldSystem->DLC1HunterHQWorld = RE::TESForm::LookupByEditorID("DLC1HunterHQWorld")->As<RE::TESWorldSpace>();	

	}

	void LoadMiscForms()
	{
		auto playerStatus = Utility::GetSingleton();
		playerStatus->Survival_ModeToggle = RE::TESForm::LookupByEditorID("Survival_ModeToggle")->As<RE::TESGlobal>();
		playerStatus->Survival_ModeEnabled = RE::TESForm::LookupByEditorID("Survival_ModeEnabled")->As<RE::TESGlobal>();
		playerStatus->SMI_WasInOblivion = RE::TESForm::LookupByEditorID("SMI_WasInOblivion")->As<RE::TESGlobal>();

		playerStatus->Survival_abLowerCarryWeightSpell = RE::TESForm::LookupByEditorID("Survival_abLowerCarryWeightSpell")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialNord = RE::TESForm::LookupByEditorID("Survival_abRacialNord")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialAltmer = RE::TESForm::LookupByEditorID("Survival_abRacialAltmer")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialOrc = RE::TESForm::LookupByEditorID("Survival_abRacialOrc")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialBosmer = RE::TESForm::LookupByEditorID("Survival_abRacialBosmer")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialDunmer = RE::TESForm::LookupByEditorID("Survival_abRacialDunmer")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialKhajiit = RE::TESForm::LookupByEditorID("Survival_abRacialKhajiit")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialArgonianRawMeat = RE::TESForm::LookupByEditorID("Survival_abRacialArgonianRawMeat")->As<RE::SpellItem>();
		playerStatus->Survival_abRacialKhajiitRawMeat = RE::TESForm::LookupByEditorID("Survival_abRacialKhajiitRawMeat")->As<RE::SpellItem>();
		playerStatus->Survival_abWarmthTorch = RE::TESForm::LookupByEditorID("Survival_abWarmthTorch")->As<RE::SpellItem>();
		playerStatus->Survival_OverencumberedSpell = RE::TESForm::LookupByEditorID("Survival_OverencumberedSpell")->As<RE::SpellItem>();

		playerStatus->Survival_ModeEnabledShared = RE::TESForm::LookupByEditorID("Survival_ModeEnabledShared")->As<RE::TESGlobal>();
		playerStatus->Survival_ModeCanBeEnabled = RE::TESForm::LookupByEditorID("Survival_ModeCanBeEnabled")->As<RE::TESGlobal>();

		playerStatus->Survival_OblivionAreaMessage = RE::TESForm::LookupByEditorID("Survival_OblivionAreaMessage")->As<RE::BGSMessage>();
		playerStatus->Survival_OblivionCells = RE::TESForm::LookupByEditorID("Survival_OblivionCells")->As<RE::BGSListForm>();
		playerStatus->Survival_OblivionLocations = RE::TESForm::LookupByEditorID("Survival_OblivionLocations")->As<RE::BGSListForm>();
		playerStatus->Survival_OblivionAreas = RE::TESForm::LookupByEditorID("Survival_OblivionAreas")->As<RE::BGSListForm>();

		playerStatus->Survival_ColdInteriorCells = RE::TESForm::LookupByEditorID("Survival_ColdInteriorCells")->As<RE::BGSListForm>();
		playerStatus->Survival_ColdInteriorLocations = RE::TESForm::LookupByEditorID("Survival_ColdInteriorLocations")->As<RE::BGSListForm>();
		playerStatus->Survival_InteriorAreas = RE::TESForm::LookupByEditorID("Survival_InteriorAreas")->As<RE::BGSListForm>();
		
		RE::SpellItem* isVampireSpell = RE::TESForm::LookupByEditorID("SMI_VampireSpell")->As<RE::SpellItem>();
		playerStatus->IsVampireConditions = &isVampireSpell->effects[0]->conditions;
		playerStatus->DA16 = RE::TESForm::LookupByEditorID("DA16")->As<RE::TESQuest>();

		RE::SpellItem* regionInfoSpell = RE::TESForm::LookupByEditorID("Survival_RegionInfoSpell")->As<RE::SpellItem>();

		playerStatus->IsInWarmArea = &regionInfoSpell->effects[0]->conditions;
		playerStatus->IsInCoolArea = &regionInfoSpell->effects[1]->conditions;
		playerStatus->IsInFreezingArea = &regionInfoSpell->effects[2]->conditions;
		playerStatus->IsInFallForestFreezingArea = &regionInfoSpell->effects[3]->conditions;
		playerStatus->IsInPineForestFreezingArea = &regionInfoSpell->effects[4]->conditions;
		playerStatus->IsInReachArea = &regionInfoSpell->effects[5]->conditions;

		/*auto flameCloak = new RE::TESConditionItem;
		flameCloak->next = nullptr;
		flameCloak->data.comparisonValue.f = 1.0f;
		flameCloak->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kHasMagicEffect;
		flameCloak->data.functionData.params[0] = fireCloakEffect;

		playerStatus->HasFlameCloakCondition = flameCloak;*/
	}
};
