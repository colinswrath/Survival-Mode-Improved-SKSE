#pragma once

#include "NeedBase.h"

class NeedHunger: public NeedBase
{
public:

	RE::TESSound* Survival_HungerASM;
	RE::TESSound* Survival_HungerBSM;
	RE::TESSound* Survival_HungerCSM;
	RE::TESSound* Survival_HungerDSM;

	static NeedHunger* GetSingleton()
	{
		static NeedHunger hungerSystem;
		return &hungerSystem;
	}

	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();

		if (ticks > 0) {
			Updating = true;
			IncrementNeed(ticks);
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}

		Updating = false;
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
			PlaySFX(Survival_HungerASM, Survival_HungerASM);
		} else if (stage == 3) {
			NotifyAddEffect(NeedMessage3, NeedMessage3Decreasing, NeedSpell3, increasing);
			PlaySFX(Survival_HungerBSM, Survival_HungerBSM);
		} else if (stage == 4) {
			NotifyAddEffect(NeedMessage4, NeedMessage4Decreasing, NeedSpell4, increasing);
			PlaySFX(Survival_HungerCSM, Survival_HungerCSM);
		} else if (stage == 5) {
			NotifyAddEffect(NeedMessage5, NeedMessage5, NeedSpell5);
			PlaySFX(Survival_HungerDSM, Survival_HungerDSM);
		}
	}
};
