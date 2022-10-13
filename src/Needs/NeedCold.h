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
	RE::TESGlobal* Survival_ColdLevelInFreezingWater;
	RE::TESGlobal* Survival_ColdResistMaxValue;
	RE::TESGlobal* Survival_TemperatureLevel;

	RE::BGSMessage* Survival_ColdConditionStage0;
	RE::BGSMessage* Survival_ColdConditionStage1;
	RE::BGSMessage* Survival_ColdConditionStage2;
	RE::BGSMessage* Survival_ColdConditionStage3;
	RE::BGSMessage* Survival_ColdConditionStage4;
	RE::BGSMessage* Survival_ColdConditionStage5;

	RE::BGSListForm* Survival_AshWeather;
	RE::BGSListForm* Survival_BlizzardWeather;
	RE::BGSListForm* SMI_ColdCloudyWeather;

	const char* Survival_FreezingASD = "Survival_FreezingASD";
	const char* Survival_FreezingBSD = "Survival_FreezingBSD";
	const char* Survival_FreezingAFemaleSD = "Survival_FreezingAFemaleSD";
	const char* Survival_FreezingBFemaleSD = "Survival_FreezingBFemaleSD";

	const float MaxWarmthRatingBonusPerc = 0.80f;		//TODO-Make this an ini setting
	const float ColdMaxStageThreshold = 600.0f;
	const float ColdToRestoreInWarmArea = 20.0f;

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
		UpdateCurrentAmbientTemp(); //TODO-Here or inside of ticks?

		int ticks = GetGameTimeTicks();
		//TODO-Check heat source
		//TODO-Check freezing water
		
		if (ticks > 0) {
			//Update ambient temp
			//Display Ambient Transition Message

			IncrementNeed(ticks);
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}
	}

	void StopNeed() override
	{
		NeedBase::StopNeed();
		Survival_TemperatureLevel->value = static_cast<float>(UI_LEVEL::kNeutral);
	}

	void IncrementNeed(int ticks) override
	{
		const std::lock_guard<std::mutex> lock(update_mutex);

		float incAmount = GetNeedIncrementAmount(ticks);

		float currentNeedLevel = CurrentNeedValue->value;
		float newNeedLevel = currentNeedLevel + incAmount;

		float maxLevel = GetMaxStageValue(); 

		if (currentNeedLevel > maxLevel) {
			//Decrement
			newNeedLevel = currentNeedLevel - (ColdToRestoreInWarmArea * ticks);
			if (newNeedLevel < maxLevel) {
				newNeedLevel = maxLevel;
			} 

		} else if (newNeedLevel > maxLevel) {
			newNeedLevel = maxLevel;
		}

		CurrentNeedValue->value = newNeedLevel;
		UpdateTemperatureUI(currentNeedLevel, newNeedLevel);
		SetNeedStage(true);
		ApplyAttributePenalty();
		//Damage if max cold
	}

	float GetNeedIncrementAmount(int ticks) override
	{
		float valPerTick = GetColdPerTick();
		logger::info(FMT_STRING("Val per tick {}"), valPerTick);

		float amount = valPerTick * static_cast<float>(ticks); 
			
		if (WasSleeping)
		{
			amount *= NeedSleepRateMult->value;
			WasSleeping = false;
		}

		amount *= (1.0f - GetWarmthRatingBonus());

		return amount;
	}

	void UpdateCurrentAmbientTemp()
	{
		//TODO-Check if you are in freezing water and return high cold level if so
		auto utility = Utility::GetSingleton();
		auto currentArea = utility->GetCurrentAreaType();

		auto oldAmbientTemp = SMI_CurrentAmbientTemp->value;

		float newAmbientTemp = GetWeatherTemperature(currentArea) + GetRegionTemperature(currentArea) + GetNightPenalty(currentArea);

		if (newAmbientTemp > NeedMaxValue->value) {
			newAmbientTemp = NeedMaxValue->value;
		}

		SMI_CurrentAmbientTemp->value = newAmbientTemp;
		DisplayAmbientTempTransitionMessage(oldAmbientTemp, newAmbientTemp);
	}

	float GetWeatherTemperature(AREA_TYPE area)
	{
		auto sky = RE::Sky::GetSingleton();
		auto currentWeather = sky->currentWeather;
	
		if (currentWeather) 
		{
			if (area != AREA_TYPE::kAreaTypeInterior) 
			{
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
		if (area == AREA_TYPE::kAreaTypeChillyInterior) {
			return static_cast<float>(REGION_TEMPS::kColdLevelCoolArea);
		} else if (area == AREA_TYPE::kAreaTypeInterior) {
			return static_cast<float>(REGION_TEMPS::kColdLevelWarmArea);
		} else if (area == AREA_TYPE::kAreaTypeWarm) {
			return static_cast<float>(REGION_TEMPS::kColdLevelWarmArea);
		} else if (area == AREA_TYPE::kAreaTypeReach) {
			return static_cast<float>(REGION_TEMPS::kColdLevelReachArea);
		} else if (area == AREA_TYPE::kAreaTypeCool) {
			return static_cast<float>(REGION_TEMPS::kColdLevelCoolArea);
		} else if (area == AREA_TYPE::kAreaTypeFreezing) {
			return static_cast<float>(REGION_TEMPS::kColdLevelFreezingArea);
		} else {
			return static_cast<float>(REGION_TEMPS::kColdLevelCoolArea);	
		}
	}

	float GetWarmthRatingBonus()
	{
		auto warmthRating = Utility::GetWarmthRating(RE::PlayerCharacter::GetSingleton());
		auto totalBonus = std::clamp(warmthRating, 0.0f, Survival_ColdResistMaxValue->value);
		return MaxWarmthRatingBonusPerc * totalBonus / Survival_ColdResistMaxValue->value;
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
		auto timeScale = RE::Calendar::GetSingleton()->GetTimescale();  //TODO-Cache the timescale

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
};
