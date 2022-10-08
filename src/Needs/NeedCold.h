#pragma once

#include "NeedBase.h"

class NeedCold : public NeedBase
{
public:

	RE::TESGlobal* Survival_ColdTargetGameHoursToNumb;
	RE::TESGlobal* SMI_ColdRate;
	RE::TESGlobal* SMI_CurrentAmbientTemp;

	RE::BGSListForm* Survival_InteriorAreas;
	RE::BGSListForm* Survival_ColdInteriorLocations;
	RE::BGSListForm* Survival_ColdInteriorCells;

	RE::TESCondition* IsInWarmArea;
	RE::TESCondition* IsInCoolArea;
	RE::TESCondition* IsInFreezingArea;
	RE::TESCondition* IsInFallForestFreezingArea;
	RE::TESCondition* IsInPineForestFreezingArea;
	RE::TESCondition* IsInReachArea;

	const char* Survival_FreezingASD = "Survival_FreezingASD";
	const char* Survival_FreezingBSD = "Survival_FreezingBSD";
	const char* Survival_FreezingAFemaleSD = "Survival_FreezingAFemaleSD";
	const char* Survival_FreezingBFemaleSD = "Survival_FreezingBFemaleSD";

	static NeedCold* GetSingleton()
	{
		static NeedCold coldSystem;
		return &coldSystem;
	}

	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();

		if (ticks > 0) {
			IncrementNeed(ticks);
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());

		}
	}

	void SetCurrentAmbientTemp()
	{

	}

	int GetWeatherTemperature()
	{

	}

	int GetRegionTemperature()
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (player->GetParentCell()->IsInteriorCell() || Survival_InteriorAreas->HasForm(player->GetWorldspace()))
		{

		}
	}

	float GetNeedIncrementAmount(int ticks) override
	{
		float coldLevel = SMI_CurrentAmbientTemp->value;         
		auto timeScale = RE::Calendar::GetSingleton()->GetTimescale();  //TODO-Cache the timescale

		float targetRealTimeSecondsToNumb = Survival_ColdTargetGameHoursToNumb->value * 3600 / timeScale;
		float coldPerSecond = coldLevel / targetRealTimeSecondsToNumb;
		float deltaRealSeconds = 60 / timeScale;	//With a default time scale (20), 1 tick is 1 in game minute or ~3 irl seconds
		float valPerTick = (coldPerSecond * deltaRealSeconds) * SMI_ColdRate->value; 

		return valPerTick * ticks;
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
