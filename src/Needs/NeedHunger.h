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
	
	RE::TESGlobal* Survival_AfflictionHungerChance;
	RE::SpellItem* Survival_AfflictionWeakened;
	RE::BGSMessage* Survival_AfflictionWeakenedMsg;

	RE::BGSListForm* Survival_FoodRawMeat;
	RE::BGSKeyword* VendorItemFoodRaw;
	RE::BGSListForm* Survival_FoodPoisoningImmuneRaces;
	RE::BGSKeyword* Survival_DiseaseFoodPoisoningKeyword;
	RE::BGSMessage* Survival_FoodPoisoningMsg;
	RE::SpellItem* Survival_DiseaseFoodPoisoning;

	RE::BGSListForm* Survival_HungerResistRacesMinor;

	const float hungerDivisor = 60.0f;

	static NeedHunger* GetSingleton()
	{
		static NeedHunger hungerSystem;
		return &hungerSystem;
	}

	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();

		if (ticks > 0) {
			IncrementNeed(ticks);
			WasSleeping = false;
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}
	}

	float GetNeedIncrementAmount(int ticks) override 
	{	
		auto player = Utility::GetPlayer();
	
		float amount = 0.0f;

		//Rate is divided by 60 in order to retain old SMI balance around 1 hour updates
		amount = (NeedRate->value / hungerDivisor) * float(ticks);

		if (WasSleeping) {
			amount = amount * NeedSleepRateMult->value;
		}

		if (Survival_HungerResistRacesMinor->HasForm(player->GetRace())) {
			amount = amount * (1.0f - Survival_RacialBonusMinor->value);
		}

		return amount;
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

		 WeakenedRollCheck();
	}

	void RemoveAfflictions() override
	{
		Utility::GetPlayer()->RemoveSpell(Survival_AfflictionWeakened);
	}

	void WeakenedRollCheck()
	{
		auto player = Utility::GetPlayer();
		if (!WasSleeping && (CurrentNeedValue->value >= NeedStage5->value) && player->HasSpell(Survival_AfflictionWeakened)) {
			float rand = Utility::GetRandomFloat(0.0f, 1.0f);

			if (rand <= Survival_AfflictionHungerChance->value) {
				NotifyAddEffect(Survival_AfflictionWeakenedMsg, nullptr, Survival_AfflictionWeakened);
			}
		}
	}
};
