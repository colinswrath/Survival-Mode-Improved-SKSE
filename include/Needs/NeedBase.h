#pragma once

#include "Utility.h"
#include <mutex>
#include <algorithm>

#undef PlaySound

class NeedBase
{
public:
	RE::TESGlobal* NeedRate;
	RE::TESGlobal* CurrentNeedStage;
	RE::TESGlobal* CurrentNeedValue;
	RE::TESGlobal* NeedSleepRateMult;

	RE::TESGlobal* LastUpdateTimeStamp;

	RE::TESGlobal* Survival_RacialBonusMinor;
	RE::TESGlobal* Survival_RacialBonusMajor;

	RE::TESGlobal* NeedStage1;
	RE::TESGlobal* NeedStage2;
	RE::TESGlobal* NeedStage3;
	RE::TESGlobal* NeedStage4;
	RE::TESGlobal* NeedStage5;
	RE::TESGlobal* NeedMaxValue;

	RE::SpellItem* NeedSpell0;
	RE::SpellItem* NeedSpell1;
	RE::SpellItem* NeedSpell2;
	RE::SpellItem* NeedSpell3;
	RE::SpellItem* NeedSpell4;
	RE::SpellItem* NeedSpell5;

	RE::BGSMessage* NeedMessage0;
	RE::BGSMessage* NeedMessage1;
	RE::BGSMessage* NeedMessage1Decreasing;
	RE::BGSMessage* NeedMessage2;
	RE::BGSMessage* NeedMessage2Decreasing;
	RE::BGSMessage* NeedMessage3;
	RE::BGSMessage* NeedMessage3Decreasing;
	RE::BGSMessage* NeedMessage4;
	RE::BGSMessage* NeedMessage4Decreasing;
	RE::BGSMessage* NeedMessage5;

	bool WasSleeping;
	bool FastTravelled;
	bool CurrentlyStopped=true;

	//Attribute penalty
	RE::ActorValue ActorValPenaltyAttribute;
	RE::ActorValue NeedPenaltyAV;
	RE::TESGlobal* NeedPenaltyUIGlobal;

	RE::TESGlobal* NeedAvPenDisabled;

	std::mutex update_mutex;

	/// <summary>
	/// Update function
	/// </summary>
	void OnUpdatePass()
	{
		if (CurrentlyStopped) {
			InitializeNeed();
		}

		if (Utility::IsPlayerInDialogue() ||
			Utility::PlayerIsBeastFormRace() ||
			Utility::IsOnFlyingMount(Utility::GetPlayer())) {
			PauseNeed();
		} else {
			UpdateNeed();
		}
	}

	virtual void InitializeNeed()
	{
		SetLastTimeStamp();
		CurrentlyStopped = false;
	}

	virtual void StopNeed()
	{
		if (!CurrentlyStopped) {
			CurrentlyStopped = true;
			RemoveNeedEffects();
			RemoveAfflictions();
			CurrentNeedStage->value = -1;
		}
	}

	virtual void IncrementNeed(int ticks)
	{	
		float incAmount = GetNeedIncrementAmount(ticks);

		float newNeedLevel = CurrentNeedValue->value + incAmount;

		if (newNeedLevel > NeedMaxValue->value) {
			newNeedLevel = NeedMaxValue->value;
		}
		
		CurrentNeedValue->value = newNeedLevel;
		SetNeedStage(true);
	}

	virtual void DecreaseNeed(float amount, float minValue = 0.0f)
	{
		float newNeedLevel = std::clamp(CurrentNeedValue->value - amount, minValue, NeedMaxValue->value);

		CurrentNeedValue->value = newNeedLevel;
		SetNeedStage(false);
	}

	void SetLastTimeStamp(float timeToSet = Utility::GetCalendar()->GetCurrentGameTime() * 1440)
	{
		LastUpdateTimeStamp->value = timeToSet;
	}

protected:

	virtual void UpdateNeed() = 0;
	virtual float GetNeedIncrementAmount(int ticks) = 0;
	virtual void ApplyNeedStageEffects(bool increasing) = 0;
	virtual void RemoveAfflictions() = 0;

	int GetGameTimeTicks()
	{
		int ticks = 0;

		auto currentTimeMinutes = GetCurrentGameTimeInMinutes();

		auto lastTimeMinutes = LastUpdateTimeStamp->value;
		if (lastTimeMinutes <= 0) {
			LastUpdateTimeStamp->value = currentTimeMinutes;
			return ticks;
		}

		ticks = int((currentTimeMinutes - lastTimeMinutes));

		return ticks;
	}

	virtual void SetNeedStage(bool increasing, bool forceUpdateEffects = false)
	{
		float currentNeedValue = CurrentNeedValue->value;

		float lastStage = CurrentNeedStage->value;

		if (currentNeedValue < NeedStage1->value) {
			CurrentNeedStage->value = 0.0f;
		} else if (currentNeedValue < NeedStage2->value) {
			CurrentNeedStage->value = 1.0f;
		} else if (currentNeedValue < NeedStage3->value) {
			CurrentNeedStage->value = 2.0f;
		} else if (currentNeedValue < NeedStage4->value) {
			CurrentNeedStage->value = 3.0f;
		} else if (currentNeedValue < NeedStage5->value) {
			CurrentNeedStage->value = 4.0f;
		} else {
			CurrentNeedStage->value = 5.0f;
		}

		if (lastStage != CurrentNeedStage->value || forceUpdateEffects) {
			ApplyNeedStageEffects(increasing);
		}
	}

	virtual void RemoveNeedEffects()
	{
		auto player = Utility::GetPlayer();
		player->RemoveSpell(NeedSpell0);
		player->RemoveSpell(NeedSpell1);
		player->RemoveSpell(NeedSpell2);
		player->RemoveSpell(NeedSpell3);
		player->RemoveSpell(NeedSpell4);
		player->RemoveSpell(NeedSpell5);
	}

	void NotifyAddEffect(RE::BGSMessage* increasingMsg, RE::BGSMessage* decreasingMsg, RE::SpellItem* spell, bool increasing=true)
	{
		if (spell) {
			Utility::GetPlayer()->AddSpell(spell);
		}

		if (increasing)
			Utility::ShowNotification(increasingMsg);
		else
			Utility::ShowNotification(decreasingMsg);
	}

	virtual void PauseNeed()
	{
		SetLastTimeStamp();
	}

	virtual void PlaySFX(const char* maleSound, const char* femaleSound)
	{	
		if (Utility::GetPlayer()->GetActorBase()->GetSex() == RE::SEX::kFemale) {
			RE::PlaySound(femaleSound);
		} else {
			RE::PlaySound(maleSound);
		}
	}

	float GetCurrentGameTimeInMinutes()
	{
		return Utility::GetCalendar()->GetCurrentGameTime() * 1440;
	}
};
