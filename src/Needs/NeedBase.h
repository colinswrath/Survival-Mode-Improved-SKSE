#pragma once

#include "PlayerStatus.h"

class NeedBase
{
public:
	RE::TESGlobal* NeedRate;
	RE::TESGlobal* CurrentNeedStage;
	RE::TESGlobal* CurrentNeedValue;
	RE::TESGlobal* NeedSleepRateMult;

	RE::TESGlobal* LastUpdateTimeStamp;

	RE::TESGlobal* NeedAttributePenaltyPercent;

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
	bool Updating;

	/// <summary>
	/// Update function
	/// </summary>
	void OnUpdatePass()
	{
		auto status = PlayerStatus::GetSingleton();
		//TODO- Dont update if you are:
		//InOblivion
		//InCombat
		//InDialogue (maybe)
		//InJail
		//BeastForm, WW or VL
		if (!status->PlayerIsInCombat()) {
			logger::info("Updating need");
			UpdateNeed();
		} else {
			SetLastTimeStamp();
		}
	}

	virtual void UpdateNeed(){};

	virtual void InitializeNeed()
	{
		SetLastTimeStamp();
	}

	virtual void StopNeed()
	{
		RemoveNeedEffects();
		CurrentNeedStage->value = -1;
	}

	/// <summary>
	/// Increment the need value based on the delta and need rate
	/// </summary>
	virtual void IncrementNeed(int ticks)
	{	
		float incAmount = GetNeedIncrementAmount(ticks);

		float newNeedLevel = CurrentNeedValue->value + incAmount;

		if (newNeedLevel > NeedMaxValue->value) 
			newNeedLevel = NeedMaxValue->value;
		else
			CurrentNeedValue->value = newNeedLevel;

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

	virtual float GetNeedIncrementAmount(int ticks)
	{
		float amount = 0;

		//Rate is divided by 60 in order to retain old SMI balance around 1 hour updates
		amount = (NeedRate->value/60.0f) * float(ticks);	//TODO-Abstract 60 to sub classes
		
		if (WasSleeping) {
			amount = amount * NeedSleepRateMult->value;
			WasSleeping = false;
		} 

		return amount;
	}

	virtual void ApplyNeedStageEffects(bool increasing)
	{
		RemoveNeedEffects();
		float stage = CurrentNeedStage->value;
		
		if (stage == 0) {
			NotifyAddEffect(NeedMessage0,NeedMessage0,NeedSpell0);
		} else if (stage == 1) {
			NotifyAddEffect(NeedMessage1,NeedMessage1Decreasing,NeedSpell1,increasing);
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

	void SetLastTimeStamp(float timeToSet = RE::Calendar::GetSingleton()->GetCurrentGameTime() * 1440) 
	{
		logger::info("setting timestamp");

		LastUpdateTimeStamp->value = timeToSet;
		logger::info("set timestamp");
	}

	void NotifyAddEffect(RE::BGSMessage* increasingMsg, RE::BGSMessage* decreasingMsg, RE::SpellItem* spell, bool increasing=true)
	{
		RE::PlayerCharacter::GetSingleton()->AddSpell(spell);
		if (increasing)
			ShowNotification(increasingMsg);
		else
			ShowNotification(decreasingMsg);
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

	//TODO-Apply SFX, Apply Rumble
	
};
