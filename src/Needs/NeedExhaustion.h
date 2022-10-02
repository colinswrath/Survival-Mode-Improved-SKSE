#pragma once

#include "NeedBase.h"

class NeedExhaustion : public NeedBase
{
public:
	RE::SpellItem* Rested;
	RE::SpellItem* WellRested;

	RE::BGSMessage* WellRestedMessage;
	RE::BGSMessage* RestedMessage;

	RE::TESQuest* PlayerSleepQuest;

	RE::TESGlobal* Survival_ExhaustionRestorePerHour;

	static NeedExhaustion* GetSingleton()
	{
		static NeedExhaustion fatigueSystem;
		return &fatigueSystem;
	}

	void InitializeNeed() override
	{
		NeedBase::InitializeNeed();
		PlayerSleepQuest->Stop();
	}

	void StopNeed() override
	{
		NeedBase::StopNeed();
		PlayerSleepQuest->Start();
	}


	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();
		if (ticks > 0) {

			Updating = true;
			if (!WasSleeping) {
				IncrementNeed(ticks);
			}

			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}
		Updating = false;
	}
	
	void DecreaseExhaustion(float hoursPassed)
	{
		DecrementNeed(hoursPassed * Survival_ExhaustionRestorePerHour->value);
		WasSleeping = false;
	}

	void ApplyNeedStageEffects(bool increasing) override
	{
		RemoveNeedEffects();
		float stage = CurrentNeedStage->value;
		
		if (stage == 0) {
			if (PlayerStatus::GetSingleton()->PlayerCanGetWellRested()) {
				NotifyAddEffect(WellRestedMessage, WellRestedMessage, WellRested);
			} else {
				NotifyAddEffect(RestedMessage, RestedMessage, Rested);	
			}

		} else if (stage == 1) {
			NotifyAddEffect(NeedMessage1, NeedMessage1Decreasing, NeedSpell1, increasing);
		} else if (stage == 2) {
			NotifyAddEffect(NeedMessage2, NeedMessage2Decreasing, NeedSpell2, increasing);
		} else if (stage == 3) {
			NotifyAddEffect(NeedMessage3, NeedMessage3Decreasing, NeedSpell3, increasing);
		} else if (stage == 4) {
			NotifyAddEffect(NeedMessage4, NeedMessage4Decreasing, NeedSpell4, increasing);
		} else if (stage == 5) {
			NotifyAddEffect(NeedMessage5, NeedMessage5, NeedSpell5);
		}
	}

	void RemoveNeedEffects() override
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		player->RemoveSpell(WellRested);
		player->RemoveSpell(Rested);
		player->RemoveSpell(NeedSpell1);
		player->RemoveSpell(NeedSpell2);
		player->RemoveSpell(NeedSpell3);
		player->RemoveSpell(NeedSpell4);
		player->RemoveSpell(NeedSpell5);
	}
};
