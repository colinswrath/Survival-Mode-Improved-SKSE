#pragma once

#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "Needs/NeedCold.h"
#include "Utility.h"

#undef AddForm

class FormLoader
{
public:
	const std::string_view smiPluginName = "SurvivalModeImproved.esp";
	const std::string_view smEslPluginName = "ccqdrsse001-survivalmode.esl";
	const std::string_view causePluginName = "ccbgssse067-daedinv.esm";
	const std::string_view updatePluginName = "Update.esm";
	const std::string_view skyrimPluginName = "Skyrim.esm";
	const std::string_view dgPluginName = "Dawnguard.esm";
	const std::string_view hfPluginName = "HearthFires.esm";
	const std::string_view campfirePluginName = "Campfire.esm";
	const std::string_view campsitePluginName = "Campsite.esp";
	const std::string_view obsidianPluginName = "ObsidianWeathers.esp";
	const std::string_view undeathPluginName = "Undeath.esp";
	const std::string_view brumaPluginName = "BSHeartland.esm";
	const std::string_view wyrmstoothPluginName = "Wyrmstooth.esp";
	const std::string_view simonrimHealthRegenPluginName = "BladeAndBluntHealth.esp";

	static FormLoader* GetSingleton()
	{
		static FormLoader formLoader;
		return &formLoader;
	}

	void LoadSettings();

	void LoadAllForms() 
	{
		logger::info("Loading all forms.");

		const auto dataHandler = RE::TESDataHandler::GetSingleton();

		if (!dataHandler->LookupLoadedLightModByName("ccQDRSSE001-SurvivalMode.esl"))
			return;

		LoadHungerForms(dataHandler);
		logger::info("Hunger forms are loaded.");
		LoadFatigueForms(dataHandler);
		logger::info("Fatigue forms are loaded.");
		LoadColdForms(dataHandler);
		logger::info("Cold forms are loaded.");
		LoadMiscForms(dataHandler);
		logger::info("All forms are loaded.");
		LoadCompatibilityForms(dataHandler);
		logger::info("Compatibility forms are loaded.");
	}

	void LoadHungerForms(RE::TESDataHandler* dataHandler)
	{	
		auto hungerSystem = NeedHunger::GetSingleton();

		hungerSystem->NeedPenaltyAV = RE::ActorValue::kVariable02;
		hungerSystem->ActorValPenaltyAttribute = RE::ActorValue::kStamina;
		hungerSystem->NeedPenaltyUIGlobal = dataHandler->LookupForm(RE::FormID(0x2EDF), updatePluginName)->As<RE::TESGlobal>();

		hungerSystem->NeedSpell0 = dataHandler->LookupForm(RE::FormID(0x876), smEslPluginName)->As<RE::SpellItem>();
		hungerSystem->NeedSpell1 = dataHandler->LookupForm(RE::FormID(0x87E), smEslPluginName)->As<RE::SpellItem>();
		hungerSystem->NeedSpell2 = dataHandler->LookupForm(RE::FormID(0x880), smEslPluginName)->As<RE::SpellItem>();
		hungerSystem->NeedSpell3 = dataHandler->LookupForm(RE::FormID(0x881), smEslPluginName)->As<RE::SpellItem>();
		hungerSystem->NeedSpell4 = dataHandler->LookupForm(RE::FormID(0x886), smEslPluginName)->As<RE::SpellItem>();
		hungerSystem->NeedSpell5 = dataHandler->LookupForm(RE::FormID(0x885), smEslPluginName)->As<RE::SpellItem>();

		hungerSystem->NeedStage1 = dataHandler->LookupForm(RE::FormID(0x806), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->NeedStage2 = dataHandler->LookupForm(RE::FormID(0x802), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->NeedStage3 = dataHandler->LookupForm(RE::FormID(0x803), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->NeedStage4 = dataHandler->LookupForm(RE::FormID(0x804), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->NeedStage5 = dataHandler->LookupForm(RE::FormID(0x805), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->NeedMaxValue = dataHandler->LookupForm(RE::FormID(0x80C), smEslPluginName)->As<RE::TESGlobal>();

		hungerSystem->NeedRate = dataHandler->LookupForm(RE::FormID(0x829), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedStage = dataHandler->LookupForm(RE::FormID(0xA14), smiPluginName)->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedValue = dataHandler->LookupForm(RE::FormID(0x81A), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->LastUpdateTimeStamp = dataHandler->LookupForm(RE::FormID(0xA15), smiPluginName)->As<RE::TESGlobal>();
		hungerSystem->NeedSleepRateMult = dataHandler->LookupForm(RE::FormID(0x823), smEslPluginName)->As<RE::TESGlobal>();

		hungerSystem->NeedMessage0 = dataHandler->LookupForm(RE::FormID(0x8C1), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage1 = dataHandler->LookupForm(RE::FormID(0x8D8), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage1Decreasing = dataHandler->LookupForm(RE::FormID(0x941), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage2 = dataHandler->LookupForm(RE::FormID(0x8CD), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage2Decreasing = dataHandler->LookupForm(RE::FormID(0x8BA), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage3 = dataHandler->LookupForm(RE::FormID(0x8B8), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage3Decreasing = dataHandler->LookupForm(RE::FormID(0x8C2), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage4 = dataHandler->LookupForm(RE::FormID(0x8B6), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage4Decreasing = dataHandler->LookupForm(RE::FormID(0x8BC), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->NeedMessage5 = dataHandler->LookupForm(RE::FormID(0x8BB), smEslPluginName)->As<RE::BGSMessage>();

		hungerSystem->Survival_FoodRestoreHungerVerySmall = dataHandler->LookupForm(RE::FormID(0x2EE1), updatePluginName)->As<RE::EffectSetting>();
		hungerSystem->Survival_FoodRestoreHungerSmall = dataHandler->LookupForm(RE::FormID(0x2EE2), updatePluginName)->As<RE::EffectSetting>();
		hungerSystem->Survival_FoodRestoreHungerMedium = dataHandler->LookupForm(RE::FormID(0x2EE3), updatePluginName)->As<RE::EffectSetting>();
		hungerSystem->Survival_FoodRestoreHungerLarge = dataHandler->LookupForm(RE::FormID(0x2EE4), updatePluginName)->As<RE::EffectSetting>();

		hungerSystem->Survival_HungerRestoreVerySmallAmount = dataHandler->LookupForm(RE::FormID(0x8DE), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->Survival_HungerRestoreSmallAmount = dataHandler->LookupForm(RE::FormID(0x82D), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->Survival_HungerRestoreMediumAmount = dataHandler->LookupForm(RE::FormID(0x82B), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->Survival_HungerRestoreLargeAmount = dataHandler->LookupForm(RE::FormID(0x82C), smEslPluginName)->As<RE::TESGlobal>();

		hungerSystem->Survival_HungerResistRacesMinor = dataHandler->LookupForm(RE::FormID(0x8A5), smEslPluginName)->As<RE::BGSListForm>();
		hungerSystem->Survival_RacialBonusMinor = dataHandler->LookupForm(RE::FormID(0x820), smEslPluginName)->As<RE::TESGlobal>();

		hungerSystem->Survival_FoodRawMeat = dataHandler->LookupForm(RE::FormID(0x8B0), smEslPluginName)->As<RE::BGSListForm>();
		hungerSystem->VendorItemFoodRaw = dataHandler->LookupForm(RE::FormID(0xA0E56), skyrimPluginName)->As<RE::BGSKeyword>();
		hungerSystem->Survival_DiseaseFoodPoisoningKeyword = dataHandler->LookupForm(RE::FormID(0x83F), smEslPluginName)->As<RE::BGSKeyword>();
		hungerSystem->Survival_FoodPoisoningImmuneRaces = dataHandler->LookupForm(RE::FormID(0x9AA), smEslPluginName)->As<RE::BGSListForm>();
		hungerSystem->Survival_FoodPoisoningMsg = dataHandler->LookupForm(RE::FormID(0x93F), smEslPluginName)->As<RE::BGSMessage>();
		hungerSystem->Survival_DiseaseFoodPoisoning = dataHandler->LookupForm(RE::FormID(0x918), smEslPluginName)->As<RE::SpellItem>();

		hungerSystem->Survival_AfflictionWeakened = dataHandler->LookupForm(RE::FormID(0x910), smEslPluginName)->As<RE::SpellItem>();
		hungerSystem->Survival_AfflictionHungerChance = dataHandler->LookupForm(RE::FormID(0x8E5), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->Survival_AfflictionWeakenedMsg = dataHandler->LookupForm(RE::FormID(0x933), smEslPluginName)->As<RE::BGSMessage>();

		hungerSystem->Survival_HelpShown_Hunger = dataHandler->LookupForm(RE::FormID(0x8E2), smEslPluginName)->As<RE::TESGlobal>();
		hungerSystem->Survival_HelpHungerHigh = dataHandler->LookupForm(RE::FormID(0x945), smEslPluginName)->As<RE::BGSMessage>();
	}

	void LoadFatigueForms(RE::TESDataHandler* dataHandler)
	{
		auto fatigueSystem = NeedExhaustion::GetSingleton();

		fatigueSystem->NeedPenaltyAV = RE::ActorValue::kVariable03;
		fatigueSystem->ActorValPenaltyAttribute = RE::ActorValue::kMagicka;
		fatigueSystem->NeedPenaltyUIGlobal = dataHandler->LookupForm(RE::FormID(0x2EE0), updatePluginName)->As<RE::TESGlobal>();

		fatigueSystem->WellRested = RE::TESForm::LookupByID(RE::FormID(0x000FB984))->As<RE::SpellItem>();
		fatigueSystem->Rested = RE::TESForm::LookupByID(RE::FormID(0x000FB981))->As<RE::SpellItem>();

		fatigueSystem->NeedSpell1 = dataHandler->LookupForm(RE::FormID(0x878), smEslPluginName)->As<RE::SpellItem>();
		fatigueSystem->NeedSpell2 = dataHandler->LookupForm(RE::FormID(0x879), smEslPluginName)->As<RE::SpellItem>();
		fatigueSystem->NeedSpell3 = dataHandler->LookupForm(RE::FormID(0x87A), smEslPluginName)->As<RE::SpellItem>();
		fatigueSystem->NeedSpell4 = dataHandler->LookupForm(RE::FormID(0x87B), smEslPluginName)->As<RE::SpellItem>();
		fatigueSystem->NeedSpell5 = dataHandler->LookupForm(RE::FormID(0x87D), smEslPluginName)->As<RE::SpellItem>();

		fatigueSystem->NeedStage1 = dataHandler->LookupForm(RE::FormID(0xA17), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->NeedStage2 = dataHandler->LookupForm(RE::FormID(0xA18), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->NeedStage3 = dataHandler->LookupForm(RE::FormID(0xA19), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->NeedStage4 = dataHandler->LookupForm(RE::FormID(0xA1A), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->NeedStage5 = dataHandler->LookupForm(RE::FormID(0xA1B), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->NeedMaxValue = dataHandler->LookupForm(RE::FormID(0x84A), smEslPluginName)->As<RE::TESGlobal>();

		fatigueSystem->WellRestedMessage = dataHandler->LookupForm(RE::FormID(0xFD80E), skyrimPluginName)->As<RE::BGSMessage>();
		fatigueSystem->RestedMessage = dataHandler->LookupForm(RE::FormID(0xFD80D), skyrimPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage1 = dataHandler->LookupForm(RE::FormID(0x8CF), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage1Decreasing = dataHandler->LookupForm(RE::FormID(0x935), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage2 = dataHandler->LookupForm(RE::FormID(0x8D0), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage2Decreasing = dataHandler->LookupForm(RE::FormID(0x8C8), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage3 = dataHandler->LookupForm(RE::FormID(0x8CE), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage3Decreasing = dataHandler->LookupForm(RE::FormID(0x8C6), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage4 = dataHandler->LookupForm(RE::FormID(0x8C7), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage4Decreasing = dataHandler->LookupForm(RE::FormID(0x8C9), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->NeedMessage5 = dataHandler->LookupForm(RE::FormID(0x8BD), smEslPluginName)->As<RE::BGSMessage>();

		fatigueSystem->NeedRate = dataHandler->LookupForm(RE::FormID(0x824), smEslPluginName)->As<RE::TESGlobal>();
		fatigueSystem->CurrentNeedStage = dataHandler->LookupForm(RE::FormID(0xA1C), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->CurrentNeedValue = dataHandler->LookupForm(RE::FormID(0x816), smEslPluginName)->As<RE::TESGlobal>();
		fatigueSystem->LastUpdateTimeStamp = dataHandler->LookupForm(RE::FormID(0xA16), smiPluginName)->As<RE::TESGlobal>();
		fatigueSystem->NeedSleepRateMult = dataHandler->LookupForm(RE::FormID(0x823), smEslPluginName)->As<RE::TESGlobal>();

		fatigueSystem->PlayerSleepQuest = dataHandler->LookupForm(RE::FormID(0xFC1A2), skyrimPluginName)->As<RE::TESQuest>();
		fatigueSystem->Survival_ExhaustionRestorePerHour = dataHandler->LookupForm(RE::FormID(0x80E), smEslPluginName)->As<RE::TESGlobal>();

		fatigueSystem->Survival_ExhaustionResistRacesMajor = dataHandler->LookupForm(RE::FormID(0x8A6), smEslPluginName)->As<RE::BGSListForm>();
		fatigueSystem->Survival_ExhaustionResistRacesMinor = dataHandler->LookupForm(RE::FormID(0x8A7), smEslPluginName)->As<RE::BGSListForm>();
		fatigueSystem->Survival_RacialBonusMinor = dataHandler->LookupForm(RE::FormID(0x822), smEslPluginName)->As<RE::TESGlobal>();
		fatigueSystem->Survival_RacialBonusMajor = dataHandler->LookupForm(RE::FormID(0x820), smEslPluginName)->As<RE::TESGlobal>();
		
		fatigueSystem->Survival_ExhaustionOverEncumberedMult = dataHandler->LookupForm(RE::FormID(0x8E3), smEslPluginName)->As<RE::TESGlobal>();

		fatigueSystem->Survival_AfflictionAddledMsg = dataHandler->LookupForm(RE::FormID(0x934), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->Survival_AfflictionExhaustionChance = dataHandler->LookupForm(RE::FormID(0x8E4), smEslPluginName)->As<RE::TESGlobal>();
		fatigueSystem->Survival_AfflictionAddled = dataHandler->LookupForm(RE::FormID(0x911), smEslPluginName)->As<RE::SpellItem>();

		fatigueSystem->MarriageRested = dataHandler->LookupForm(RE::FormID(0xCDA1D), skyrimPluginName)->As<RE::SpellItem>();
		fatigueSystem->MarriageRestedMessage = dataHandler->LookupForm(RE::FormID(0xFD80F), skyrimPluginName)->As<RE::BGSMessage>();

		fatigueSystem->Survival_HelpExhaustionHigh = dataHandler->LookupForm(RE::FormID(0x943), smEslPluginName)->As<RE::BGSMessage>();
		fatigueSystem->Survival_HelpShown_Exhaustion = dataHandler->LookupForm(RE::FormID(0x8E1), smEslPluginName)->As<RE::TESGlobal>();

		fatigueSystem->BYOHAdoptionRestedMessageMale = dataHandler->LookupForm(RE::FormID(0x2F55), hfPluginName)->As<RE::BGSMessage>();
		fatigueSystem->BYOHAdoptionRestedMessageFemale = dataHandler->LookupForm(RE::FormID(0x4293), hfPluginName)->As<RE::BGSMessage>();
		fatigueSystem->BYOHAdoptionSleepAbilityMale = dataHandler->LookupForm(RE::FormID(0x2F54), hfPluginName)->As<RE::SpellItem>();
		fatigueSystem->BYOHAdoptionSleepAbilityFemale = dataHandler->LookupForm(RE::FormID(0x4292), hfPluginName)->As<RE::SpellItem>();
	}

	void LoadColdForms(RE::TESDataHandler* dataHandler)
	{
		auto coldSystem = NeedCold::GetSingleton();

		coldSystem->NeedPenaltyAV = RE::ActorValue::kVariable04;
		coldSystem->ActorValPenaltyAttribute = RE::ActorValue::kHealth;
		coldSystem->NeedPenaltyUIGlobal = dataHandler->LookupForm(RE::FormID(0x2EDE), updatePluginName)->As<RE::TESGlobal>();

		coldSystem->Survival_ColdTargetGameHoursToNumb = dataHandler->LookupForm(RE::FormID(0x813), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->SMI_ColdRate = dataHandler->LookupForm(RE::FormID(0x800), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->SMI_CurrentAmbientTemp = dataHandler->LookupForm(RE::FormID(0xD1D), smiPluginName)->As<RE::TESGlobal>();

		coldSystem->NeedSpell0 = dataHandler->LookupForm(RE::FormID(0x890), smEslPluginName)->As<RE::SpellItem>();
		coldSystem->NeedSpell1 = dataHandler->LookupForm(RE::FormID(0x86E), smEslPluginName)->As<RE::SpellItem>();
		coldSystem->NeedSpell2 = dataHandler->LookupForm(RE::FormID(0x891), smEslPluginName)->As<RE::SpellItem>();
		coldSystem->NeedSpell3 = dataHandler->LookupForm(RE::FormID(0x86D), smEslPluginName)->As<RE::SpellItem>();
		coldSystem->NeedSpell4 = dataHandler->LookupForm(RE::FormID(0x870), smEslPluginName)->As<RE::SpellItem>();
		coldSystem->NeedSpell5 = dataHandler->LookupForm(RE::FormID(0x871), smEslPluginName)->As<RE::SpellItem>();

		coldSystem->Survival_AfflictionFrostbitten = dataHandler->LookupForm(RE::FormID(0x913), smEslPluginName)->As<RE::SpellItem>();
		coldSystem->Survival_FreezingWaterDamage = dataHandler->LookupForm(RE::FormID(0x882), smEslPluginName)->As<RE::SpellItem>();

		coldSystem->NeedStage1 = dataHandler->LookupForm(RE::FormID(0xD20), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->NeedStage2 = dataHandler->LookupForm(RE::FormID(0xD21), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->NeedStage3 = dataHandler->LookupForm(RE::FormID(0xD22), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->NeedStage4 = dataHandler->LookupForm(RE::FormID(0xD23), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->NeedStage5 = dataHandler->LookupForm(RE::FormID(0xD24), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->NeedMaxValue = dataHandler->LookupForm(RE::FormID(0x84B), smEslPluginName)->As<RE::TESGlobal>();

		coldSystem->Survival_ColdConditionStage0 = dataHandler->LookupForm(RE::FormID(0x93B), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage1 = dataHandler->LookupForm(RE::FormID(0x93D), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage2 = dataHandler->LookupForm(RE::FormID(0x940), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage3 = dataHandler->LookupForm(RE::FormID(0x938), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage4 = dataHandler->LookupForm(RE::FormID(0x939), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->Survival_ColdConditionStage5 = dataHandler->LookupForm(RE::FormID(0x8DC), smEslPluginName)->As<RE::BGSMessage>();

		coldSystem->NeedMessage0 = dataHandler->LookupForm(RE::FormID(0x8DA), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage1 = dataHandler->LookupForm(RE::FormID(0x8D9), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage1Decreasing = dataHandler->LookupForm(RE::FormID(0x942), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage2 = dataHandler->LookupForm(RE::FormID(0x8CC), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage2Decreasing = dataHandler->LookupForm(RE::FormID(0x8D4), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage3 = dataHandler->LookupForm(RE::FormID(0x8D7), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage3Decreasing = dataHandler->LookupForm(RE::FormID(0x8CA), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage4 = dataHandler->LookupForm(RE::FormID(0x8CB), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage4Decreasing = dataHandler->LookupForm(RE::FormID(0x8D6), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->NeedMessage5 = dataHandler->LookupForm(RE::FormID(0x8D2), smEslPluginName)->As<RE::BGSMessage>();

		coldSystem->Survival_AfflictionFrostbittenMsg = dataHandler->LookupForm(RE::FormID(0x932), smEslPluginName)->As<RE::BGSMessage>();
		coldSystem->Survival_WaterFreezingMessage = dataHandler->LookupForm(RE::FormID(0x8D1), smEslPluginName)->As<RE::BGSMessage>();

		coldSystem->CurrentNeedStage = dataHandler->LookupForm(RE::FormID(0xD1E), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->CurrentNeedValue = dataHandler->LookupForm(RE::FormID(0x81B), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_ColdLevelInFreezingWater = dataHandler->LookupForm(RE::FormID(0x815), smEslPluginName)->As<RE::TESGlobal>();

		coldSystem->Survival_AshWeather = dataHandler->LookupForm(RE::FormID(0x8A8), smEslPluginName)->As<RE::BGSListForm>();
		coldSystem->Survival_BlizzardWeather = dataHandler->LookupForm(RE::FormID(0x8A9), smEslPluginName)->As<RE::BGSListForm>();
		coldSystem->SMI_ColdCloudyWeather = dataHandler->LookupForm(RE::FormID(0x80A), smiPluginName)->As<RE::BGSListForm>();
		coldSystem->Survival_WarmUpObjectsList = dataHandler->LookupForm(RE::FormID(0x8AA), smEslPluginName)->As<RE::BGSListForm>();

		coldSystem->Survival_FoodRestoreCold = dataHandler->LookupForm(RE::FormID(0x2EE5), updatePluginName)->As<RE::EffectSetting>();

		coldSystem->LastUpdateTimeStamp = dataHandler->LookupForm(RE::FormID(0xD1F), smiPluginName)->As<RE::TESGlobal>();
		coldSystem->NeedSleepRateMult = dataHandler->LookupForm(RE::FormID(0x823), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_ColdResistMaxValue = dataHandler->LookupForm(RE::FormID(0x8E8), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_TemperatureLevel = dataHandler->LookupForm(RE::FormID(0x2EDD), updatePluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_ColdLevelInFreezingWater = dataHandler->LookupForm(RE::FormID(0x815), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_LastWaterFreezingMsgTime = dataHandler->LookupForm(RE::FormID(0x812), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_ColdRestoreSmallAmount = dataHandler->LookupForm(RE::FormID(0x819), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_ColdRestoreMediumAmount = dataHandler->LookupForm(RE::FormID(0x818), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->SMI_VampireColdRate = dataHandler->LookupForm(RE::FormID(0x901), smiPluginName)->As<RE::TESGlobal>();

		coldSystem->DLC1HunterHQWorld = dataHandler->LookupForm(RE::FormID(0x1DB8), dgPluginName)->As<RE::TESWorldSpace>();
		coldSystem->Survival_HelpShown_Cold = dataHandler->LookupForm(RE::FormID(0x8E0), smEslPluginName)->As<RE::TESGlobal>();
		coldSystem->Survival_HelpColdHigh = dataHandler->LookupForm(RE::FormID(0x944), smEslPluginName)->As<RE::BGSMessage>();
	}

	void LoadMiscForms(RE::TESDataHandler* dataHandler)
	{
		auto utility = Utility::GetSingleton();
		utility->Survival_ModeToggle = dataHandler->LookupForm(RE::FormID(0x828), smEslPluginName)->As<RE::TESGlobal>();
		utility->Survival_ModeEnabled = dataHandler->LookupForm(RE::FormID(0x826), smEslPluginName)->As<RE::TESGlobal>();

		utility->Survival_abLowerCarryWeightSpell = dataHandler->LookupForm(RE::FormID(0x887), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abLowerRegenSpell = dataHandler->LookupForm(RE::FormID(0x982), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialNord = dataHandler->LookupForm(RE::FormID(0x88B), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialAltmer = dataHandler->LookupForm(RE::FormID(0x874), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialOrc = dataHandler->LookupForm(RE::FormID(0x873), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialBosmer = dataHandler->LookupForm(RE::FormID(0x88F), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialDunmer = dataHandler->LookupForm(RE::FormID(0x88E), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialKhajiit = dataHandler->LookupForm(RE::FormID(0x872), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialArgonianRawMeat = dataHandler->LookupForm(RE::FormID(0x88D), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abRacialKhajiitRawMeat = dataHandler->LookupForm(RE::FormID(0x877), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_abWarmthTorch = dataHandler->LookupForm(RE::FormID(0x983), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_OverencumberedSpell = dataHandler->LookupForm(RE::FormID(0x917), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_OblivionDisplaySpell = dataHandler->LookupForm(RE::FormID(0x90E), smEslPluginName)->As<RE::SpellItem>();

		utility->Survival_ModeEnabledShared = dataHandler->LookupForm(RE::FormID(0x314A), updatePluginName)->As<RE::TESGlobal>();
		utility->Survival_ModeCanBeEnabled = dataHandler->LookupForm(RE::FormID(0xA0F), smEslPluginName)->As<RE::TESGlobal>();

		utility->Survival_OblivionAreaMessage = dataHandler->LookupForm(RE::FormID(0x937), smEslPluginName)->As<RE::BGSMessage>();
		utility->Survival_OblivionCells = dataHandler->LookupForm(RE::FormID(0x92B), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_OblivionLocations = dataHandler->LookupForm(RE::FormID(0x92D), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_OblivionAreas = dataHandler->LookupForm(RE::FormID(0x92E), smEslPluginName)->As<RE::BGSListForm>();

		utility->Survival_ColdInteriorCells = dataHandler->LookupForm(RE::FormID(0x92A), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_ColdInteriorLocations = dataHandler->LookupForm(RE::FormID(0x92C), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_InteriorAreas = dataHandler->LookupForm(RE::FormID(0x92F), smEslPluginName)->As<RE::BGSListForm>();
		
		RE::SpellItem* isWerewolfSpell = dataHandler->LookupForm(RE::FormID(0xA01), smiPluginName)->As<RE::SpellItem>();
		utility->IsWerewolfConditions = &isWerewolfSpell->effects[0]->conditions;

		RE::SpellItem* isVampireSpell = dataHandler->LookupForm(RE::FormID(0x80E), smiPluginName)->As<RE::SpellItem>();
		utility->IsVampireConditions = &isVampireSpell->effects[0]->conditions;

		utility->DA16 = dataHandler->LookupForm(RE::FormID(0x242AF), skyrimPluginName)->As<RE::TESQuest>();
		utility->MQ101 = dataHandler->LookupForm(RE::FormID(0x3372B), skyrimPluginName)->As<RE::TESQuest>();

		utility->WerewolfFeedRestoreHealth = dataHandler->LookupForm(RE::FormID(0xEC356), skyrimPluginName)->As<RE::EffectSetting>();
		utility->DA11AbFortifyHealth = dataHandler->LookupForm(RE::FormID(0x10F814), skyrimPluginName)->As<RE::EffectSetting>();
		utility->Survival_FireCloakFreezingWaterDesc = dataHandler->LookupForm(RE::FormID(0x2EE9), updatePluginName)->As<RE::EffectSetting>();
		utility->RelationshipMarriageFIN = dataHandler->LookupForm(RE::FormID(0x21382), skyrimPluginName)->As<RE::TESQuest>();
		utility->BYOHRelationshipAdoption = dataHandler->LookupForm(RE::FormID(0x42B4), hfPluginName)->As<RE::TESQuest>();
		RE::SpellItem* regionInfoSpell = dataHandler->LookupForm(RE::FormID(0x87F), smEslPluginName)->As<RE::SpellItem>();

		utility->LocTypeInn = dataHandler->LookupForm(RE::FormID(0x1CB87), skyrimPluginName)->As<RE::BGSKeyword>();
		utility->LocTypePlayerHouse = dataHandler->LookupForm(RE::FormID(0xFC1A3), skyrimPluginName)->As<RE::BGSKeyword>();

		utility->Survival_TempleBlessingCostPerk = dataHandler->LookupForm(RE::FormID(0x9EE), smEslPluginName)->As<RE::BGSPerk>();		

		utility->HelpManualPC = dataHandler->LookupForm(RE::FormID(0x163), skyrimPluginName)->As<RE::BGSListForm>();
		utility->HelpManualXBox = dataHandler->LookupForm(RE::FormID(0x165), skyrimPluginName)->As<RE::BGSListForm>();

		utility->Survival_HelpSurvivalModeLong = dataHandler->LookupForm(RE::FormID(0x9C1), smEslPluginName)->As<RE::BGSMessage>();
		utility->Survival_HelpSurvivalModeLongXbox = dataHandler->LookupForm(RE::FormID(0x9EB), smEslPluginName)->As<RE::BGSMessage>();

		utility->Survival_DiseaseGutworm = dataHandler->LookupForm(RE::FormID(0x915), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_DiseaseGutworm2 = dataHandler->LookupForm(RE::FormID(0x989), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_DiseaseGutworm3 = dataHandler->LookupForm(RE::FormID(0x98A), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_DiseaseGreenspore = dataHandler->LookupForm(RE::FormID(0x912), smEslPluginName)->As<RE::SpellItem>();
		utility->Survival_DiseaseBrownRot = dataHandler->LookupForm(RE::FormID(0x914), smEslPluginName)->As<RE::SpellItem>();

		utility->Survival_GutwormCarryingRaces = dataHandler->LookupForm(RE::FormID(0x9A9), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_GreensporeCarryingRaces = dataHandler->LookupForm(RE::FormID(0x9A5), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_BrownRotCarryingRaces = dataHandler->LookupForm(RE::FormID(0x9A4), smEslPluginName)->As<RE::BGSListForm>();
		utility->Survival_SurvivalDiseases = dataHandler->LookupForm(RE::FormID(0x9A6), smEslPluginName)->As<RE::BGSListForm>();

		utility->UnboundQuest = dataHandler->LookupForm(RE::FormID(0x3372B), skyrimPluginName)->As<RE::TESQuest>();

		utility->IsInWarmArea = &regionInfoSpell->effects[0]->conditions;
		utility->IsInCoolArea = &regionInfoSpell->effects[1]->conditions;
		utility->IsInFreezingArea = &regionInfoSpell->effects[2]->conditions;
		utility->IsInFallForestFreezingArea = &regionInfoSpell->effects[3]->conditions;
		utility->IsInPineForestFreezingArea = &regionInfoSpell->effects[4]->conditions;
		utility->IsInReachArea = &regionInfoSpell->effects[5]->conditions;

		if (dataHandler->LookupLoadedModByName(brumaPluginName)) {
			logger::info("Loading Bruma region data");
			utility->CYRWeatherMountainsSnow = dataHandler->LookupForm(RE::FormID(0x6311F), brumaPluginName)->As<RE::TESRegion>();
			utility->CYRWeatherBruma = dataHandler->LookupForm(RE::FormID(0x63102), brumaPluginName)->As<RE::TESRegion>();
			utility->CYRWeatherGreatForestNorth = dataHandler->LookupForm(RE::FormID(0x7EE90), brumaPluginName)->As<RE::TESRegion>();
		
			auto brumaInFreezingArea = new RE::TESConditionItem;
			brumaInFreezingArea->next = nullptr;
			brumaInFreezingArea->data.comparisonValue.f = 1.0f;
			brumaInFreezingArea->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			brumaInFreezingArea->data.functionData.params[0] = utility->CYRWeatherMountainsSnow;

			auto brumaInComfortableArea = new RE::TESConditionItem;
			brumaInComfortableArea->next = nullptr;
			brumaInComfortableArea->data.comparisonValue.f = 1.0f;
			brumaInComfortableArea->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			brumaInComfortableArea->data.functionData.params[0] = utility->CYRWeatherGreatForestNorth;

			auto brumaInCityArea = new RE::TESConditionItem;
			brumaInCityArea->next = nullptr;
			brumaInCityArea->data.comparisonValue.f = 1.0f;
			brumaInCityArea->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			brumaInCityArea->data.functionData.params[0] = utility->CYRWeatherBruma;

			utility->BrumaIsCityArea = brumaInCityArea;
			utility->BrumaIsInFreezingArea = brumaInFreezingArea;
			utility->BrumaIsComfortableArea = brumaInComfortableArea;	
			logger::info("Bruma region data loaded");

		}

		if (dataHandler->LookupLoadedModByName(wyrmstoothPluginName)) {
			logger::info("Loading Wyrmstooth region data");

			utility->WyrmstoothSteampools = dataHandler->LookupForm(RE::FormID(0x57654F), wyrmstoothPluginName)->As<RE::TESRegion>();
			utility->WyrmstoothForest = dataHandler->LookupForm(RE::FormID(0x58CB3B), wyrmstoothPluginName)->As<RE::TESRegion>();
			utility->WyrmstoothMarsh = dataHandler->LookupForm(RE::FormID(0x57654E), wyrmstoothPluginName)->As<RE::TESRegion>();

			utility->WyrmstoothWorldspace = dataHandler->LookupForm(RE::FormID(0xD62), wyrmstoothPluginName)->As<RE::TESWorldSpace>();
			logger::info("Loaded Wyrmstooth forms");
		
			auto wtWarm = new RE::TESConditionItem;
			wtWarm->next = nullptr;
			wtWarm->data.comparisonValue.f = 1.0f;
			wtWarm->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			wtWarm->data.functionData.params[0] = utility->WyrmstoothSteampools;

			auto wtWarmCond = new RE::TESCondition;
			wtWarmCond->head = wtWarm;
			
			utility->WTIsInWarmArea = wtWarmCond;

			//------------------------------------------------------------
			auto wtNotMarsh = new RE::TESConditionItem;
			wtNotMarsh->next = nullptr;
			wtNotMarsh->data.comparisonValue.f = 0.0f;
			wtNotMarsh->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			wtNotMarsh->data.functionData.params[0] = utility->WyrmstoothMarsh;
			
			auto wtNotForest = new RE::TESConditionItem;
			wtNotForest->next = wtNotMarsh;
			wtNotForest->data.comparisonValue.f = 0.0f;
			wtNotForest->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			wtNotForest->data.functionData.params[0] = utility->WyrmstoothForest;
			
			auto wtNotSteampools = new RE::TESConditionItem;
			wtNotSteampools->next = wtNotForest;
			wtNotSteampools->data.comparisonValue.f = 0.0f;
			wtNotSteampools->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			wtNotSteampools->data.functionData.params[0] = utility->WyrmstoothSteampools;
			
			auto wtWorldspaceFreezing = new RE::TESConditionItem;
			wtWorldspaceFreezing->next = wtNotSteampools;
			wtWorldspaceFreezing->data.comparisonValue.f = 1.0f;
			wtWorldspaceFreezing->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetInWorldspace;
			wtWorldspaceFreezing->data.functionData.params[0] = utility->WyrmstoothWorldspace;

			auto wtFreezingCond = new RE::TESCondition;
			wtFreezingCond->head = wtWorldspaceFreezing;

			utility->WTIsInFreezingArea = wtFreezingCond;

			//------------------------------------------------------------

			auto wtMarsh = new RE::TESConditionItem;
			wtMarsh->next = nullptr;
			wtMarsh->data.comparisonValue.f = 1.0f;
			wtMarsh->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			wtMarsh->data.functionData.params[0] = utility->WyrmstoothMarsh;

			auto wtForest = new RE::TESConditionItem;
			wtForest->next = wtMarsh;
			wtForest->data.comparisonValue.f = 1.0f;
			wtForest->data.flags.isOR = true;
			wtForest->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPlayerInRegion;
			wtForest->data.functionData.params[0] = utility->WyrmstoothForest;

			auto wtWorldspaceCool = new RE::TESConditionItem;
			wtWorldspaceCool->next = wtForest;
			wtWorldspaceCool->data.comparisonValue.f = 1.0f;
			wtWorldspaceCool->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetInWorldspace;
			wtWorldspaceCool->data.functionData.params[0] = utility->WyrmstoothWorldspace;

			auto wtCoolCond = new RE::TESCondition;
			wtCoolCond->head = wtWorldspaceCool;

			utility->WTIsInCoolArea = wtCoolCond;
			//------------------------------------------------------------
			logger::info("Wyrmstooth region data loaded");
			
		}		

		auto inJail = new RE::TESConditionItem;
		inJail->next = nullptr;
		inJail->data.comparisonValue.f = 0.0f;
		inJail->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kGetDaysInJail;
		inJail->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kGreaterThan;

		utility->IsInJailCondition = inJail;

	}

	void LoadCompatibilityForms(RE::TESDataHandler* dataHandler)
	{
		auto warmupList = NeedCold::GetSingleton()->Survival_WarmUpObjectsList;
		auto util = Utility::GetSingleton();
		if (dataHandler->LookupLoadedModByName(campsitePluginName)) {
			auto campfire = dataHandler->LookupForm(RE::FormID(0x5902), campsitePluginName);

			if (campfire && !warmupList->HasForm(campfire)) {
				warmupList->AddForm(campfire);
			}
		}
			
		if (dataHandler->LookupLoadedModByName(campfirePluginName)) {
			auto cracklingDw = dataHandler->LookupForm(RE::FormID(0x40013), campfirePluginName);
			if (cracklingDw && !warmupList->HasForm(cracklingDw))
				warmupList->AddForm(cracklingDw);
			auto cracklingFw = dataHandler->LookupForm(RE::FormID(0x328B9), campfirePluginName);
			if (cracklingFw && !warmupList->HasForm(cracklingFw))
				warmupList->AddForm(cracklingFw);
			auto flickBooks = dataHandler->LookupForm(RE::FormID(0x328A8), campfirePluginName);
			if (flickBooks && !warmupList->HasForm(flickBooks))
				warmupList->AddForm(flickBooks);
			auto flickBranch = dataHandler->LookupForm(RE::FormID(0x328A6), campfirePluginName);
			if (flickBranch && !warmupList->HasForm(flickBranch))
				warmupList->AddForm(flickBranch);
			auto flickKind = dataHandler->LookupForm(RE::FormID(0x5C8D8), campfirePluginName);
			if (flickKind && !warmupList->HasForm(flickKind))
				warmupList->AddForm(flickKind);
			auto roarDw = dataHandler->LookupForm(RE::FormID(0x33E67), campfirePluginName);
			if (roarDw && !warmupList->HasForm(roarDw))
				warmupList->AddForm(roarDw);
			auto roarFw = dataHandler->LookupForm(RE::FormID(0x33E69), campfirePluginName);
			if (roarFw && !warmupList->HasForm(roarFw))
				warmupList->AddForm(roarFw);
			auto roarWFw = dataHandler->LookupForm(RE::FormID(0x6ABB2), campfirePluginName);
			if (roarWFw && !warmupList->HasForm(roarWFw))
				warmupList->AddForm(roarWFw);
			auto fragKind = dataHandler->LookupForm(RE::FormID(0x5C8D6), campfirePluginName);
			if (fragKind && !warmupList->HasForm(fragKind))
				warmupList->AddForm(fragKind);
			auto fragBooks = dataHandler->LookupForm(RE::FormID(0x32334), campfirePluginName);
			if (fragBooks && !warmupList->HasForm(fragBooks))
				warmupList->AddForm(fragBooks);
			auto fragBranch = dataHandler->LookupForm(RE::FormID(0x32333), campfirePluginName);
			if (fragBranch && !warmupList->HasForm(fragBranch))
				warmupList->AddForm(fragBranch);		
		}

		if (dataHandler->LookupLoadedModByName(obsidianPluginName)) {

			auto cloudy = NeedCold::GetSingleton()->SMI_ColdCloudyWeather; 
			auto w1 = dataHandler->LookupForm(RE::FormID(0x0010E1E3), skyrimPluginName);
			if (w1 && !cloudy->HasForm(w1))
				cloudy->AddForm(w1);	
			auto w2 = dataHandler->LookupForm(RE::FormID(0x0010A235), skyrimPluginName);
			if (w2 && !cloudy->HasForm(w2))
				cloudy->AddForm(w2);	
			auto w3 = dataHandler->LookupForm(RE::FormID(0x0010E1E5), skyrimPluginName);
			if (w3 && !cloudy->HasForm(w3))
				cloudy->AddForm(w3);	
			auto w4 = dataHandler->LookupForm(RE::FormID(0x0010E1E8), skyrimPluginName);
			if (w4 && !cloudy->HasForm(w4))
				cloudy->AddForm(w4);	
			auto w5 = dataHandler->LookupForm(RE::FormID(0x0010E1EF), skyrimPluginName);
			if (w5 && !cloudy->HasForm(w5))
				cloudy->AddForm(w5);	
			auto w6 = dataHandler->LookupForm(RE::FormID(0x0010A233), skyrimPluginName);
			if (w6 && !cloudy->HasForm(w6))
				cloudy->AddForm(w6);	
			auto w7 = dataHandler->LookupForm(RE::FormID(0x0010A232), skyrimPluginName);
			if (w7 && !cloudy->HasForm(w7))
				cloudy->AddForm(w7);	
			auto w8 = dataHandler->LookupForm(RE::FormID(0x00104AB4), skyrimPluginName);
			if (w8 && !cloudy->HasForm(w8))
				cloudy->AddForm(w8);	
			auto w9 = dataHandler->LookupForm(RE::FormID(0x00010E0B), dgPluginName);
			if (w9 && !cloudy->HasForm(w9))
				cloudy->AddForm(w9);	
			auto w10 = dataHandler->LookupForm(RE::FormID(0x00010E0E), dgPluginName);
			if (w10 && !cloudy->HasForm(w10))
				cloudy->AddForm(w10);	
		}

		if (dataHandler->LookupLoadedModByName(undeathPluginName)) {
			auto lichPerk = dataHandler->LookupForm(RE::FormID(0x3326D5), undeathPluginName);
			if (lichPerk) {
				util->Undeath_LichPerk = lichPerk->As<RE::BGSPerk>();
			}
		}

		if (dataHandler->LookupLoadedModByName(causePluginName)) {
			auto deadLandsWorldspace = dataHandler->LookupForm(RE::FormID(0xE1592), causePluginName);
			auto deadLandsLoc = dataHandler->LookupForm(RE::FormID(0x33F39), causePluginName);

			if (deadLandsWorldspace && !util->Survival_OblivionAreas->HasForm(deadLandsWorldspace)) {
				util->Survival_OblivionAreas->AddForm(deadLandsWorldspace);
			}

			if (deadLandsLoc && !util->Survival_OblivionAreas->HasForm(deadLandsLoc)) {
				util->Survival_OblivionLocations->AddForm(deadLandsLoc);
			}
		}

		auto simonrimGlobal = dataHandler->LookupForm(RE::FormID(0xD25), smiPluginName)->As<RE::TESGlobal>();
		if (dataHandler->LookupLoadedLightModByName(simonrimHealthRegenPluginName)) {
			simonrimGlobal->value = 1.0f;
		} else {
			simonrimGlobal->value = 0.0f;	
		}
	}

	//Cache commonly called addresses to avoid address lib overhead
	void CacheGameAddresses()
	{
		auto utility = Utility::GetSingleton();

		//Cache player singleton address
		utility->PlayerSingletonAddress = RELOCATION_ID(517014, 403521).address();
		utility->UISingletonAddress = RELOCATION_ID(514178, 400327).address();
		utility->CalendarSingletonAddress = RELOCATION_ID(514287, 400447).address();
		utility->MenuControlsSingletonAddress = RELOCATION_ID(515124, 401263).address();
		utility->GetWarmthRatingAddress = RELOCATION_ID(25834, 26394).address();
		utility->DoCombatSpellApplyAddress = RELOCATION_ID(37666, 38620).address();
		utility->EnableFtAddress = RELOCATION_ID(54946, 55563).address();
		utility->IsFtEnabledAddress = RELOCATION_ID(54848, 55481).address();
	}
};
