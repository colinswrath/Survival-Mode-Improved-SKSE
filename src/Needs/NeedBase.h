#pragma once

#include "PlayerStatus.h"
#include <mutex>

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
		
		auto status = PlayerStatus::GetSingleton();
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

	virtual float GetNeedDivisor() = 0;

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
			CurrentNeedStage->value = 0;
		} else if (currentNeedValue < NeedStage2->value) {
			CurrentNeedStage->value = 1;
		} else if (currentNeedValue < NeedStage3->value) {
			CurrentNeedStage->value = 2;
		} else if (currentNeedValue < NeedStage4->value) {
			CurrentNeedStage->value = 3;
		} else if (currentNeedValue < NeedStage5->value) {
			CurrentNeedStage->value = 4;
		} else {
			CurrentNeedStage->value = 5;
		}

		if (lastStage != CurrentNeedStage->value) {
			ApplyNeedStageEffects(increasing);
		}
	}

	virtual void ApplyNeedStageEffects(bool increasing) = 0;

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

	float GetMaxAttributeAv(RE::PlayerCharacter* player)
	{
		return (player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, ActorValPenaltyAttribute) + player->GetActorBase()->GetPermanentActorValue(ActorValPenaltyAttribute));
	}

	virtual float GetPenaltyPercentAmount()
	{
		auto penalty = (CurrentNeedValue->value - NeedStage2->value - 1) / (NeedMaxValue->value - NeedStage2->value - 1);
		if (penalty < 0) {
			penalty = 0;
		} else if (penalty > 1.0) {
			penalty = 1.0;
		}

		return penalty;
	}

	void SetAttributePenaltyUIGlobal(float penaltyPerc) 
	{
		auto newVal = penaltyPerc * 100;
		
		if (newVal < 0) {
			newVal = 0;
		} else if (newVal > 100) {
			newVal = 100;
		}

		NeedPenaltyUIGlobal->value = newVal;
	}

	virtual float GetNeedIncrementAmount(int ticks, float racialBonus)
	{
		float amount = 0;
		float racialBonus = 0;
		
		//Rate is divided by 60 in order to retain old SMI balance around 1 hour updates
		amount = (NeedRate->value/GetNeedDivisor()) * float(ticks);	


	
		if (WasSleeping) {
			amount = amount * NeedSleepRateMult->value * (1.00000 - racialBonus);
			WasSleeping = false;
		} 
		else {
			amount = amount * float(ticks) * (1.00000 - racialBonus);
		}

		return amount;
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
			ShowNotification(increasingMsg);
		else
			ShowNotification(decreasingMsg);
	}

	virtual void PlaySFX(const char* maleSound, const char* femaleSound)
	{	
		if (RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSex() == RE::SEX::kFemale) {
			RE::PlaySound(femaleSound);
		} else {
			RE::PlaySound(maleSound);
		}
	}

	void ShowNotification(RE::BGSMessage* msg)
	{
		RE::BSString messageDesc;
		msg->GetDescription(messageDesc, msg);
		RE::DebugNotification(messageDesc.c_str());
	}

	float GetCurrentGameTimeInMinutes()
	{
		return RE::Calendar::GetSingleton()->GetCurrentGameTime() * 1440;
	}
};
