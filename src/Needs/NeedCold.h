#pragma once

#include "NeedBase.h"
#include "Utility.h"
#include <algorithm>

enum class REGION_TEMPS
{
	kColdLevelWarmArea = 35,
	kColdLevelCoolArea = 125,
	kColdLevelReachArea = 100,
	kColdLevelFreezingArea = 250
};

enum class WEATHER_TEMPS
{
	kComfortableTemp = 0,
	kRainyTemp = 50,
	kColdCloudySnowTemp = 100,
	kSnowTemp = 200,
	kBlizzardTemp = 450,
};

enum class UI_LEVEL
{
	kNeutral = 0,
	kNearHeat = 1,
	kWarm = 2,
	kCold = 3,
	kFreezing = 4
};

//TODO-Remove logs in the future, they are for testing
class NeedCold : public NeedBase
{
public:

	RE::TESGlobal* Survival_ColdTargetGameHoursToNumb;
	RE::TESGlobal* SMI_ColdRate;
	RE::TESGlobal* SMI_CurrentAmbientTemp;
	RE::TESGlobal* Survival_ColdResistMaxValue;
	RE::TESGlobal* Survival_TemperatureLevel;
	RE::TESGlobal* Survival_ColdRestoreSmallAmount;

	RE::BGSMessage* Survival_ColdConditionStage0;
	RE::BGSMessage* Survival_ColdConditionStage1;
	RE::BGSMessage* Survival_ColdConditionStage2;
	RE::BGSMessage* Survival_ColdConditionStage3;
	RE::BGSMessage* Survival_ColdConditionStage4;
	RE::BGSMessage* Survival_ColdConditionStage5;

	RE::BGSListForm* Survival_AshWeather;
	RE::BGSListForm* Survival_BlizzardWeather;
	RE::BGSListForm* SMI_ColdCloudyWeather;
	RE::BGSListForm* Survival_WarmUpObjectsList;

	RE::TESGlobal* Survival_ColdLevelInFreezingWater;
	RE::SpellItem* Survival_FreezingWaterDamage;
	RE::TESGlobal* Survival_LastWaterFreezingMsgTime;
	RE::BGSMessage* Survival_WaterFreezingMessage;
	RE::TESWorldSpace* DLC1HunterHQWorld;

	const char* FXFreezingWaterSoundFX = "FXFreezingWaterSoundFX";
	const char* FXFreezingWaterSoundFXFemale = "FXFreezingWaterSoundFXFemale";

	const char* Survival_FreezingASD = "Survival_FreezingASD";
	const char* Survival_FreezingBSD = "Survival_FreezingBSD";
	const char* Survival_FreezingAFemaleSD = "Survival_FreezingAFemaleSD";
	const char* Survival_FreezingBFemaleSD = "Survival_FreezingBFemaleSD";

	const float MaxWarmthRatingBonusPerc = 0.80f;		//TODO-Make these an ini setting
	const float ColdMaxStageThreshold = 600.0f;
	const float ColdToRestoreInWarmArea = 1.5f;
	
	float SeasonMults[12] = 
	{ 
		1.5f,
		1.4f, 
		1.3f, 
		1.2f, 
		1.1f, 
		1.0f, 
		0.8f, 
		1.0f, 
		1.3f, 
		1.4f, 
		1.5f, 
		1.6f 
	};

	//Night mod - TODO - Make ini setting
	float WarmAreaNightMod = 25.0f;
	float CoolAreaNightMod = 50.0f;
	float FreezingAreaNightMod = 100.0f;

	static NeedCold* GetSingleton()
	{
		static NeedCold coldSystem;
		return &coldSystem;
	}

	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();
		auto utility = Utility::GetSingleton();
		auto currentArea = utility->GetCurrentAreaType();
		bool nearHeat = false;

		//Todo-Abstract heat/freezing water check to survivalMode.cpp and utility
		if (!FreezingWaterCheck(currentArea)) {
			nearHeat = HeatSourceCheck();
			UpdateCurrentAmbientTemp(currentArea);
		}	
		
		if (ticks > 0) {
			if (!nearHeat) {
				IncrementNeed(ticks);
			} else {
				DecrementNeedHeat(ticks);
			}

			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}
	}

	void StopNeed() override
	{
		NeedBase::StopNeed();
		Survival_TemperatureLevel->value = static_cast<float>(UI_LEVEL::kNeutral);
	}

	void UpdateCurrentAmbientTemp(AREA_TYPE currentArea)
	{
		auto oldAmbientTemp = SMI_CurrentAmbientTemp->value;
		float newAmbientTemp = 0.0f;

		newAmbientTemp = std::clamp(GetWeatherTemperature(currentArea) + GetRegionTemperature(currentArea) + GetNightPenalty(currentArea), 0.0f, NeedMaxValue->value);

		SMI_CurrentAmbientTemp->value = newAmbientTemp;
		DisplayAmbientTempTransitionMessage(oldAmbientTemp, newAmbientTemp);
	}

	//In this case its more of an "update" than a definite increment 
	void IncrementNeed(int ticks) override
	{
		const std::lock_guard<std::mutex> lock(update_mutex);
		float currentNeedLevel = CurrentNeedValue->value;

		float incAmount = GetNeedIncrementAmount(ticks);

		float maxLevel = GetMaxStageValue();

		if (currentNeedLevel > maxLevel) {
			DecreaseNeed((ColdToRestoreInWarmArea * ticks), maxLevel);
		} else {
			IncreaseColdLevel(incAmount, NeedMaxValue->value);
		}

		//Damage if max cold
	}

	void DecrementNeedHeat(int ticks) 
	{
		auto decAmount = Survival_ColdRestoreSmallAmount->value * ticks;

		float min = 0.0f;
		if (SMI_CurrentAmbientTemp->value >= static_cast<float>(AREA_TYPE::kAreaTypeFreezing) && Utility::GetPlayer()) {
			min = NeedStage1->value;
		}

		DecreaseNeed(decAmount, min);
		SetUIHeat(min);
	}

	void IncreaseColdLevel(float increaseAmount, float max) 
	{
		float currentNeedLevel = CurrentNeedValue->value;

		float newNeedLevel = std::clamp(currentNeedLevel + increaseAmount, 0.0f, max);
		
		CurrentNeedValue->value = newNeedLevel;
		UpdateTemperatureUI(currentNeedLevel, newNeedLevel);
		SetNeedStage(true);
		ApplyAttributePenalty();
	}

	void DecreaseNeed(float decreaseAmount, float min = 0.0f) override
	{
		float currentNeedLevel = CurrentNeedValue->value;

		float newNeedLevel = std::clamp(currentNeedLevel - decreaseAmount, min, NeedMaxValue->value);

		CurrentNeedValue->value = newNeedLevel;
		UpdateTemperatureUI(currentNeedLevel, newNeedLevel);
		SetNeedStage(true);
		ApplyAttributePenalty();
	}

	float GetNeedIncrementAmount(int ticks) override
	{
		float valPerTick = GetColdPerTick();

		float amount = valPerTick * static_cast<float>(ticks); 
			
		if (WasSleeping){
			amount *= NeedSleepRateMult->value;
			WasSleeping = false;
		}
		amount *= (1.0f - GetWarmthRatingBonus());
		return amount;
	}

	float GetWeatherTemperature(AREA_TYPE area)
	{
		auto sky = RE::Sky::GetSingleton();
		auto currentWeather = sky->currentWeather;
	
		if (currentWeather) {
			if (area != AREA_TYPE::kAreaTypeInterior) {
				bool coldCloudy = SMI_ColdCloudyWeather->HasForm(currentWeather);
				if (coldCloudy && (area == AREA_TYPE::kAreaTypeFreezing)) {
					return static_cast<float>(WEATHER_TEMPS::kSnowTemp);	
				} else if (coldCloudy) {
					return static_cast<float>(WEATHER_TEMPS::kColdCloudySnowTemp);
				}

				if (currentWeather->data.flags.any(RE::TESWeather::WeatherDataFlag::kSnow)) {

					if (Survival_AshWeather->HasForm(currentWeather)) {
						return static_cast<float>(WEATHER_TEMPS::kComfortableTemp);
					} else if (Survival_BlizzardWeather->HasForm(currentWeather)) {
						return static_cast<float>(WEATHER_TEMPS::kBlizzardTemp);
					} else {
						return static_cast<float>(WEATHER_TEMPS::kSnowTemp);
					}

				} else if (currentWeather->data.flags.any(RE::TESWeather::WeatherDataFlag::kRainy)) {
					return static_cast<float>(WEATHER_TEMPS::kRainyTemp);		
				}
			}
		}

		return static_cast<float>(WEATHER_TEMPS::kComfortableTemp);
	}

	float GetRegionTemperature(AREA_TYPE area)
	{
		auto month = Utility::GetCalendar()->GetMonth();

		switch (area) {
		case AREA_TYPE::kAreaTypeChillyInterior:
			return static_cast<float>(REGION_TEMPS::kColdLevelCoolArea);
		case AREA_TYPE::kAreaTypeInterior:
			return static_cast<float>(REGION_TEMPS::kColdLevelWarmArea);
		case AREA_TYPE::kAreaTypeWarm:
			return static_cast<float>(REGION_TEMPS::kColdLevelWarmArea) * SeasonMults[month];
		case AREA_TYPE::kAreaTypeReach:
			return static_cast<float>(REGION_TEMPS::kColdLevelReachArea) * SeasonMults[month];
		case AREA_TYPE::kAreaTypeCool:
			return static_cast<float>(REGION_TEMPS::kColdLevelCoolArea) * SeasonMults[month];
		case AREA_TYPE::kAreaTypeFreezing:
			return static_cast<float>(REGION_TEMPS::kColdLevelFreezingArea) * SeasonMults[month];
		default:
			return static_cast<float>(REGION_TEMPS::kColdLevelCoolArea) * SeasonMults[month];	
		}
	}

	float GetNightPenalty(AREA_TYPE area)
	{

		float nightPen = 0.0f;
		if (area != AREA_TYPE::kAreaTypeInterior && area != AREA_TYPE::kAreaTypeChillyInterior) {
			auto sky = RE::Sky::GetSingleton();

			if (sky) {
				auto climate = sky->currentClimate;
				if (climate) {
					auto hour = sky->currentGameHour;

					auto sunriseBegin = static_cast<float>(climate->timing.sunrise.begin);
					auto sunriseEnd = static_cast<float>(climate->timing.sunrise.end);
					auto sunsetBegin = static_cast<float>(climate->timing.sunset.begin);
					auto sunsetEnd = static_cast<float>(climate->timing.sunset.end);
					bool night = false;

					if (hour < ((sunriseBegin + sunriseEnd) / 2.0f ) / 6.0f) {		//NightPen sunrise
						night = true;
					} else if (hour > ((sunsetBegin + sunsetEnd) / 2.0f) / 6.0f) {  //NightPen sunset
						night = true;		
					} 

					if (night) {
						switch (area) {
						case AREA_TYPE::kAreaTypeWarm:
							nightPen = WarmAreaNightMod;
							break;
						case AREA_TYPE::kAreaTypeCool:
						case AREA_TYPE::kAreaTypeReach:
							nightPen = CoolAreaNightMod;
							break;
						case AREA_TYPE::kAreaTypeFreezing:
							nightPen = FreezingAreaNightMod;
							break;
						}
					}
				}
			}
		}
		return nightPen;
	}

	float GetWarmthRatingBonus()
	{
		auto warmthRating = Utility::GetWarmthRating(Utility::GetPlayer());
		auto totalBonus = std::clamp(warmthRating, 0.0f, Survival_ColdResistMaxValue->value);
		return MaxWarmthRatingBonusPerc * totalBonus / Survival_ColdResistMaxValue->value;
	}

	void DisplayAmbientTempTransitionMessage(float previousTemp, float currentTemp)
	{
		if (previousTemp < NeedStage5->value && currentTemp >= NeedStage5->value) {
			Utility::ShowNotification(Survival_ColdConditionStage5);
		} else if (previousTemp < NeedStage4->value && currentTemp >= NeedStage4->value) {
			Utility::ShowNotification(Survival_ColdConditionStage4);
		} else if (previousTemp < NeedStage3->value && currentTemp >= NeedStage3->value) {
			Utility::ShowNotification(Survival_ColdConditionStage3);
		} else if (previousTemp < NeedStage2->value && currentTemp >= NeedStage2->value) {
			Utility::ShowNotification(Survival_ColdConditionStage2);
		} else if (previousTemp < NeedStage1->value && currentTemp >= NeedStage1->value) {
			Utility::ShowNotification(Survival_ColdConditionStage1);
		} else if (previousTemp > static_cast<float>(REGION_TEMPS::kColdLevelWarmArea) && 
			currentTemp <= static_cast<float>(REGION_TEMPS::kColdLevelWarmArea)) {
			Utility::ShowNotification(Survival_ColdConditionStage0);
		}
	}

	void UpdateTemperatureUI(float oldColdVal, float newColdVal) 
	{
		UI_LEVEL uiSetting;
		if (oldColdVal == newColdVal) {
			uiSetting = UI_LEVEL::kNeutral;
		} else if (oldColdVal > newColdVal) {
			uiSetting = UI_LEVEL::kWarm;
		} else {
			if (SMI_CurrentAmbientTemp->value >= NeedStage5->value) {
				uiSetting = UI_LEVEL::kFreezing;
			} else if (SMI_CurrentAmbientTemp->value >= NeedStage3->value) {
				uiSetting = UI_LEVEL::kCold;
			} else {
				uiSetting = UI_LEVEL::kNeutral;
			}
		}

		Survival_TemperatureLevel->value = static_cast<float>(uiSetting);
	}
	
	void SetUIHeat(float min = 0.0f)
	{
		if (CurrentNeedValue->value > min) {
			Survival_TemperatureLevel->value = static_cast<float>(UI_LEVEL::kNearHeat);
		} else {
			Survival_TemperatureLevel->value = static_cast<float>(UI_LEVEL::kNeutral);
		}
	}

	float GetMaxStageValue()
	{
		if (SMI_CurrentAmbientTemp->value >= ColdMaxStageThreshold) {
			return NeedMaxValue->value;
		} else {
			return SMI_CurrentAmbientTemp->value;
		}

		//if(conditions.isBeastRace == 1 && currentColdLevel > needStage4Value)
		//needStage4Value as Float
	}

	float GetColdPerTick()
	{
		float coldLevel = SMI_CurrentAmbientTemp->value;
		auto timeScale = Utility::GetCalendar()->GetTimescale();  //TODO-Cache the timescale

		float targetRealTimeSecondsToNumb = Survival_ColdTargetGameHoursToNumb->value * 3600 / timeScale;
		float coldPerSecond = coldLevel / targetRealTimeSecondsToNumb;
		float deltaRealSeconds = 60 / timeScale;  //With a default time scale (20), 1 tick is 1 in game minute or ~3 irl seconds
		return (coldPerSecond * deltaRealSeconds) * SMI_ColdRate->value; 
	}

	void ApplyNeedStageEffects(bool increasing) override
	{
		RemoveNeedEffects();
		float stage = CurrentNeedStage->value;

		if (stage == 0) {
			NotifyAddEffect(NeedMessage0, NeedMessage0, NeedSpell0);
		} else if (stage == 1) {
			NotifyAddEffect(NeedMessage1, NeedMessage1Decreasing, NeedSpell1, increasing);
		} else if (stage == 2) {
			NotifyAddEffect(NeedMessage2, NeedMessage2Decreasing, NeedSpell2, increasing);
			PlaySFX(Survival_FreezingASD, Survival_FreezingAFemaleSD);
		} else if (stage == 3) {
			NotifyAddEffect(NeedMessage3, NeedMessage3Decreasing, NeedSpell3, increasing);
			PlaySFX(Survival_FreezingASD, Survival_FreezingAFemaleSD);
		} else if (stage == 4) {
			NotifyAddEffect(NeedMessage4, NeedMessage4Decreasing, NeedSpell4, increasing);
			PlaySFX(Survival_FreezingBSD, Survival_FreezingBFemaleSD);
		} else if (stage == 5) {
			NotifyAddEffect(NeedMessage5, NeedMessage5, NeedSpell5);
			PlaySFX(Survival_FreezingBSD, Survival_FreezingBFemaleSD);
		}
	}

	bool FreezingWaterCheck(AREA_TYPE currentArea)
	{
		auto utility = Utility::GetSingleton();
		auto player = Utility::GetPlayer();
		auto playerState = player->AsActorState();

		if (playerState->IsSwimming() && !utility->PlayerHasFlameCloak() &&
			(currentArea == AREA_TYPE::kAreaTypeFreezing || currentArea == AREA_TYPE::kAreaTypeChillyInterior || player->GetWorldspace() == DLC1HunterHQWorld)) {
			auto currentVal = Survival_LastWaterFreezingMsgTime->value;

			if (currentVal > 9 || currentVal == 0.0f) {
				PlaySFX(FXFreezingWaterSoundFX, FXFreezingWaterSoundFXFemale);
				NotifyAddEffect(Survival_WaterFreezingMessage, nullptr, Survival_FreezingWaterDamage);
				Survival_LastWaterFreezingMsgTime->value = 0.0f;
			}

			IncreaseColdLevel(NeedMaxValue->value, NeedStage3->value);  //Dont let cold fall below level 3
			SMI_CurrentAmbientTemp->value = Survival_ColdLevelInFreezingWater->value;

			Survival_LastWaterFreezingMsgTime->value = std::clamp(currentVal += 1.0f, 0.0f, 10.0f);
			return true;
		} else if (player->HasSpell(Survival_FreezingWaterDamage)) {
			player->RemoveSpell(Survival_FreezingWaterDamage);
			Survival_LastWaterFreezingMsgTime->value = 0.0f;
		}
		return false;
	}

	bool HeatSourceCheck()
	{
		auto TES = RE::TES::GetSingleton();
		auto player = Utility::GetPlayer();
		auto playerState = player->AsActorState();

		std::vector<RE::TESObjectREFR*> heatSources;

		bool nearHeat = false;
		if (TES && !player->IsRunning() && !playerState->IsSprinting() && !playerState->IsSwimming()) {
			TES->ForEachReferenceInRange(player, 580.0f, [&](RE::TESObjectREFR& b_ref) {
				if (const auto base = b_ref.GetBaseObject(); base && b_ref.Is3DLoaded()) {
					if (Survival_WarmUpObjectsList->HasForm(base)) {
						nearHeat = true;
						return RE::BSContainer::ForEachResult::kStop;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
		return nearHeat;
	}
};
