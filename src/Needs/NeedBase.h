#pragma once

#include "Utility.h"
#include <mutex>
#include <algorithm>

class NeedBase
{
public:
	RE::TESGlobal* NeedRate;
	RE::TESGlobal* CurrentNeedStage;
	RE::TESGlobal* CurrentNeedValue;
	RE::TESGlobal* NeedSleepRateMult;

	RE::TESGlobal* LastUpdateTimeStamp;

	RE::TESGlobal* NeedAttributePenaltyPercent;

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
	bool CurrentlyStopped=false;

	//Attribute penalty
	RE::ActorValue ActorValPenaltyAttribute;
	RE::ActorValue NeedPenaltyAV;
	RE::TESGlobal* NeedPenaltyUIGlobal;

	std::mutex update_mutex;

	/// <summary>
	/// Update function
	/// </summary>
	void OnUpdatePass()
	{
		
		auto status = Utility::GetSingleton();
		//TODO- Pause needs if you are:
		//InCombat
		//InDialogue (maybe)
		//InJail
		//BeastForm, WW or VL
		if (!status->PlayerIsInCombat()) {
			UpdateNeed();
		} else {
			SetLastTimeStamp();
		}
	}

	virtual void InitializeNeed()
	{
		SetLastTimeStamp();
		CurrentlyStopped = false;
	}

	virtual void StopNeed()
	{
		CurrentlyStopped = true;
		RemoveNeedEffects();
		RemoveAttributePenalty();
		CurrentNeedStage->value = -1;
	}

	/// <summary>
	/// Increment the need value based on the delta and need rate
	/// </summary>
	virtual void IncrementNeed(int ticks)
	{	
		const std::lock_guard<std::mutex> lock(update_mutex);
		float incAmount = GetNeedIncrementAmount(ticks);

		float newNeedLevel = CurrentNeedValue->value + incAmount;

		if (newNeedLevel > NeedMaxValue->value) {
			newNeedLevel = NeedMaxValue->value;
		}
		
		CurrentNeedValue->value = newNeedLevel;
		SetNeedStage(true);
		ApplyAttributePenalty();
	}

	virtual void DecrementNeed(float amount, float minValue = 0)
	{
		const std::lock_guard<std::mutex> lock(update_mutex);
		float newNeedLevel = CurrentNeedValue->value - amount;

		if (newNeedLevel < minValue) {
			newNeedLevel = minValue;
		}

		CurrentNeedValue->value = newNeedLevel;
		SetNeedStage(false);
		ApplyAttributePenalty();
	}

	void SetLastTimeStamp(float timeToSet = RE::Calendar::GetSingleton()->GetCurrentGameTime() * 1440)
	{
		LastUpdateTimeStamp->value = timeToSet;
	}

protected:

	virtual void UpdateNeed() = 0;

	virtual float GetNeedIncrementAmount(int ticks) = 0;

	virtual void ApplyNeedStageEffects(bool increasing) = 0;

	/// <summary>
	/// Get delta between time stamps in game seconds. For now, 1 tick = 1 in game minute
	/// </summary>
	/// <returns>Number of ticks that have passed since last update</returns>
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

	/// <summary>
	/// Determine the current need stage.
	/// If we are in a new stage then update the effects
	/// </summary>
	virtual void SetNeedStage(bool increasing)
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

		if (lastStage != CurrentNeedStage->value) {
			ApplyNeedStageEffects(increasing);
		}
	}

	virtual void ApplyAttributePenalty()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		float maxPenAv = GetMaxAttributeAv(player);
		float penaltyPerc = GetPenaltyPercentAmount();
		float currentPenaltyMag = player->AsActorValueOwner()->GetActorValue(NeedPenaltyAV);
		float newPenaltyMag = maxPenAv * penaltyPerc;

		if (newPenaltyMag > maxPenAv) {
			newPenaltyMag = maxPenAv;
		}
		auto magDelta = currentPenaltyMag - newPenaltyMag;

		//Set tracker av not actual damage
		player->AsActorValueOwner()->SetActorValue(NeedPenaltyAV, newPenaltyMag);

		//Damage or restore AV
		player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, ActorValPenaltyAttribute, magDelta);
		SetAttributePenaltyUIGlobal(penaltyPerc);
	}

	virtual void RemoveAttributePenalty()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		float currentPenaltyMag = player->AsActorValueOwner()->GetActorValue(NeedPenaltyAV);

		player->AsActorValueOwner()->SetActorValue(NeedPenaltyAV, 0.0f);
		player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, ActorValPenaltyAttribute, currentPenaltyMag);
	}

	float GetMaxAttributeAv(RE::PlayerCharacter* player)
	{
		return (player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, ActorValPenaltyAttribute) + player->GetActorBase()->GetPermanentActorValue(ActorValPenaltyAttribute));
	}

	virtual float GetPenaltyPercentAmount()
	{
		auto penalty = (CurrentNeedValue->value - NeedStage2->value - 1) / (NeedMaxValue->value - NeedStage2->value - 1);
		penalty = std::clamp(penalty, 0.0f, 1.0f);

		return penalty;
	}

	void SetAttributePenaltyUIGlobal(float penaltyPerc) 
	{
		auto newVal = penaltyPerc * 100.0f;
		newVal = std::clamp(newVal, 0.0f, 100.0f);

		NeedPenaltyUIGlobal->value = newVal;
	}

	/// <summary>
	/// Remove all need effects
	/// </summary>
	virtual void RemoveNeedEffects()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		player->RemoveSpell(NeedSpell0);
		player->RemoveSpell(NeedSpell1);
		player->RemoveSpell(NeedSpell2);
		player->RemoveSpell(NeedSpell3);
		player->RemoveSpell(NeedSpell4);
		player->RemoveSpell(NeedSpell5);
	}

	void NotifyAddEffect(RE::BGSMessage* increasingMsg, RE::BGSMessage* decreasingMsg, RE::SpellItem* spell, bool increasing=true)
	{
		RE::PlayerCharacter::GetSingleton()->AddSpell(spell);
		if (increasing)
			Utility::ShowNotification(increasingMsg);
		else
			Utility::ShowNotification(decreasingMsg);
	}

	virtual void PlaySFX(const char* maleSound, const char* femaleSound)
	{	
		if (RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSex() == RE::SEX::kFemale) {
			RE::PlaySound(femaleSound);
		} else {
			RE::PlaySound(maleSound);
		}
	}

	float GetCurrentGameTimeInMinutes()
	{
		return RE::Calendar::GetSingleton()->GetCurrentGameTime() * 1440;
	}
};
