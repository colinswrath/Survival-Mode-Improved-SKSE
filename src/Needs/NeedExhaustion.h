#pragma once

#include "NeedBase.h"

class NeedExhaustion : public NeedBase
{
public:
	RE::SpellItem* Rested;
	RE::SpellItem* WellRested;

	RE::BGSMessage* WellRestedMessage;
	RE::BGSMessage* RestedMessage;
	RE::BGSMessage* BYOHAdoptionRestedMessageMale;
	RE::BGSMessage* BYOHAdoptionRestedMessageFemale;
	RE::BGSMessage* Survival_HelpExhaustionHigh;

	RE::TESQuest* PlayerSleepQuest;
	
	RE::TESGlobal* Survival_ExhaustionRestorePerHour;
	RE::TESGlobal* Survival_ExhaustionOverEncumberedMult;
	RE::TESGlobal* Survival_AfflictionExhaustionChance;
	RE::TESGlobal* Survival_HelpShown_Exhaustion;

	RE::BGSListForm* Survival_ExhaustionResistRacesMajor;
	RE::BGSListForm* Survival_ExhaustionResistRacesMinor;

	RE::BGSMessage* Survival_AfflictionAddledMsg;
	RE::SpellItem* Survival_AfflictionAddled;	

	RE::SpellItem* MarriageRested;	
	RE::BGSMessage* MarriageRestedMessage;

	RE::SpellItem* BYOHAdoptionSleepAbilityMale;	
	RE::SpellItem* BYOHAdoptionSleepAbilityFemale;	

	const char* Survival_ExhaustedASD = "Survival_ExhaustedASD";
	const char* Survival_ExhaustedBSD = "Survival_ExhaustedBSD";
	const char* Survival_ExhaustedAFemaleSD = "Survival_ExhaustedAFemaleSD";
	const char* Survival_ExhaustedBFemaleSD = "Survival_ExhaustedBFemaleSD";

	const float exhaustionDivisor = 60.0f;

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
		if (!CurrentlyStopped) {
			NeedBase::StopNeed();
			PlayerSleepQuest->Start();
		}
	}

	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();
		if (ticks > 0) {
			if (!WasSleeping) {
				IncrementNeed(ticks);
			}

			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}
	}

	float GetNeedIncrementAmount(int ticks) override {
		auto player = Utility::GetPlayer();
		float amount = 0.0f;

		auto rate = NeedRate->value;

		//Rate is divided by 60 in order to retain old SMI balance around 1 hour updates
		amount = (rate / exhaustionDivisor) * float(ticks);

		if (Survival_ExhaustionResistRacesMinor->HasForm(player->GetRace())) {
			amount = amount * (1.0f - Survival_RacialBonusMinor->value);
		} else if (Survival_ExhaustionResistRacesMajor->HasForm(player->GetRace())) {
			amount = amount * (1.0f - Survival_RacialBonusMajor->value);
		}

		if (player->IsOverEncumbered()) {
			amount = amount * Survival_ExhaustionOverEncumberedMult->value;
		}

		return amount;
	}
	
	void DecreaseExhaustion(float hoursPassed)
	{
		DecreaseNeed(hoursPassed * Survival_ExhaustionRestorePerHour->value);
		CheckAdoptionBonus();
		WasSleeping = false;
	}

	void ApplyNeedStageEffects(bool increasing) override
	{
		RemoveNeedEffects();
		float stage = CurrentNeedStage->value;
		if (stage == 0 && Utility::PlayerCanGetWellRested()) {
			if (Utility::PlayerIsNearSpouse()) {  //Todo-need to see if you are near spouse
				NotifyAddEffect(MarriageRestedMessage, MarriageRestedMessage, MarriageRested);
			} else if (Utility::PlayerIsInHouseOrInn()) {  //Check inn, house
				NotifyAddEffect(WellRestedMessage, WellRestedMessage, WellRested);
			} else {
				NotifyAddEffect(RestedMessage, RestedMessage, Rested);			
			}	
		} else if (stage <= 1) {
			NotifyAddEffect(NeedMessage1, NeedMessage1Decreasing, NeedSpell1, increasing);
		} else if (stage == 2) {
			NotifyAddEffect(NeedMessage2, NeedMessage2Decreasing, NeedSpell2, increasing);
			PlaySFX(Survival_ExhaustedASD, Survival_ExhaustedAFemaleSD);
		} else if (stage == 3) {
			NotifyAddEffect(NeedMessage3, NeedMessage3Decreasing, NeedSpell3, increasing);
			PlaySFX(Survival_ExhaustedASD, Survival_ExhaustedAFemaleSD);
		} else if (stage == 4) {
			NotifyAddEffect(NeedMessage4, NeedMessage4Decreasing, NeedSpell4, increasing);
			PlaySFX(Survival_ExhaustedBSD, Survival_ExhaustedBFemaleSD);
		} else if (stage == 5) {
			NotifyAddEffect(NeedMessage5, NeedMessage5, NeedSpell5);
			PlaySFX(Survival_ExhaustedBSD, Survival_ExhaustedBFemaleSD);
		}

		if (stage >= 2 && Survival_HelpShown_Exhaustion->value == 0.0f) {
			Utility::ShowNotification(Survival_HelpExhaustionHigh, true);
			Survival_HelpShown_Exhaustion->value = 1.0f;
		}

		AddledRollCheck();
	}

	void CheckAdoptionBonus()
	{
		RemoveAdoptionBonus();
		if (Utility::PlayerIsNearAdopted()) {
			if (Utility::GetPlayer()->GetActorBase()->GetSex() == RE::SEX::kFemale) {
				NotifyAddEffect(BYOHAdoptionRestedMessageFemale,nullptr,BYOHAdoptionSleepAbilityFemale);
			} else {
				NotifyAddEffect(BYOHAdoptionRestedMessageMale, nullptr, BYOHAdoptionSleepAbilityMale);	
			}
		}
	}

	void RemoveAdoptionBonus()
	{
		if (Utility::GetPlayer()->HasSpell(BYOHAdoptionSleepAbilityFemale)) {
			Utility::GetPlayer()->RemoveSpell(BYOHAdoptionSleepAbilityFemale);
		} else if (Utility::GetPlayer()->HasSpell(BYOHAdoptionSleepAbilityMale)) {
			Utility::GetPlayer()->RemoveSpell(BYOHAdoptionSleepAbilityMale);
		}
	}

	void RemoveNeedEffects() override
	{
		auto player = Utility::GetPlayer();
		player->RemoveSpell(WellRested);
		player->RemoveSpell(Rested);
		player->RemoveSpell(NeedSpell1);
		player->RemoveSpell(NeedSpell2);
		player->RemoveSpell(NeedSpell3);
		player->RemoveSpell(NeedSpell4);
		player->RemoveSpell(NeedSpell5);
	}

	void RemoveAfflictions() override
	{
		Utility::GetPlayer()->RemoveSpell(Survival_AfflictionAddled);
	}

	void AddledRollCheck()
	{
		auto player = Utility::GetPlayer();
		if (!WasSleeping && (CurrentNeedValue->value >= NeedStage5->value) && player->HasSpell(Survival_AfflictionAddled)) {
			float rand = Utility::GetRandomFloat(0.0f, 1.0f);

			if (rand <= Survival_AfflictionExhaustionChance->value) {
				NotifyAddEffect(Survival_AfflictionAddledMsg, Survival_AfflictionAddledMsg, Survival_AfflictionAddled);
			}
		}
	}
};
