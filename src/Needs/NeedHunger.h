#pragma once

#include "NeedBase.h"

class NeedHunger: public NeedBase
{
public:

	const char* Survival_HungerASD = "Survival_HungerASD";
	const char* Survival_HungerBSD = "Survival_HungerBSD";
	const char* Survival_HungerCSD = "Survival_HungerCSD";
	const char* Survival_HungerDSD = "Survival_HungerDSD";

	RE::EffectSetting* Survival_FoodRestoreHungerVerySmall;
	RE::EffectSetting* Survival_FoodRestoreHungerSmall;
	RE::EffectSetting* Survival_FoodRestoreHungerMedium;
	RE::EffectSetting* Survival_FoodRestoreHungerLarge;

	RE::TESGlobal* Survival_HungerRestoreLargeAmount;
	RE::TESGlobal* Survival_HungerRestoreMediumAmount;
	RE::TESGlobal* Survival_HungerRestoreSmallAmount;
	RE::TESGlobal* Survival_HungerRestoreVerySmallAmount;

	RE::BGSListForm* Survival_HungerResistanceRacesMinor;

	const float hungerDivisor = 60.0f;

	float GetNeedDivisor() override
	{
		return hungerDivisor;
	}

	static NeedHunger* GetSingleton()
	{
		static NeedHunger hungerSystem;
		return &hungerSystem;
	}

	float GetNeedIncrementAmount(int ticks) override 
	{	
		auto player = RE::PlayerCharacter::GetSingleton();
		float amount = 0.0f;

		//Rate is divided by 60 in order to retain old SMI balance around 1 hour updates
		amount = (NeedRate->value / GetNeedDivisor()) * float(ticks);

		if (WasSleeping) {
			amount = amount * NeedSleepRateMult->value;
			WasSleeping = false;
		}

		if (Survival_HungerResistanceRacesMinor->HasForm(player->GetRace())) {
			amount = amount * (1.0f - Survival_RacialBonusMinor->value);
		}

		return amount;
	}


	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();

		if (ticks > 0) {
			IncrementNeed(ticks);
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}
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
			PlaySFX(Survival_HungerASD, Survival_HungerASD);
		} else if (stage == 3) {
			NotifyAddEffect(NeedMessage3, NeedMessage3Decreasing, NeedSpell3, increasing);
			PlaySFX(Survival_HungerBSD, Survival_HungerBSD);
		} else if (stage == 4) {
			NotifyAddEffect(NeedMessage4, NeedMessage4Decreasing, NeedSpell4, increasing);
			PlaySFX(Survival_HungerCSD, Survival_HungerCSD);
		} else if (stage == 5) {
			NotifyAddEffect(NeedMessage5, NeedMessage5, NeedSpell5);
			PlaySFX(Survival_HungerDSD, Survival_HungerDSD);
		}
	}
};
