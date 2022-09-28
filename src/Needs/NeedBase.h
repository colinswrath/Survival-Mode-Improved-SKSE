#pragma once

class NeedBase
{
public:
	RE::TESGlobal* NeedRate;
	RE::TESGlobal* CurrentNeedStage;
	RE::TESGlobal* CurrentNeedLevel;

	RE::TESGlobal* LastUpdateTimeStamp;

	RE::TESGlobal* NeedStage1;
	RE::TESGlobal* NeedStage2;
	RE::TESGlobal* NeedStage3;
	RE::TESGlobal* NeedStage4;
	RE::TESGlobal* NeedStage5;

	RE::SpellItem* NeedSpell0;
	RE::SpellItem* NeedSpell1;
	RE::SpellItem* NeedSpell2;
	RE::SpellItem* NeedSpell3;
	RE::SpellItem* NeedSpell4;
	RE::SpellItem* NeedSpell5;

	void OnUpdateNeed()
	{
		IncrementNeed();
		SetNeedStage();
	}

	/// <summary>
	/// Increment the need value based on the delta and need rate
	/// </summary>
	void IncrementNeed()
	{
		int ticks = GetGameTimeTicks();

		float incAmount = GetNeedIncrementAmount(ticks);

		float newNeedLevel = CurrentNeedLevel->value + incAmount;

		if (newNeedLevel > NeedStage5->value) {
			newNeedLevel = NeedStage5->value;
		}

		CurrentNeedLevel->value = newNeedLevel;
	}

	/// <summary>
	/// Determine the current need stage
	/// </summary>
	void SetNeedStage()
	{
		float currentNeedValue = CurrentNeedLevel->value;

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
			ApplyNeedStageEffects();
		}
	}

	float GetNeedIncrementAmount(int ticks)
	{
		float amount = 0;

		amount = NeedRate->value * float(ticks);

		return amount;
	}

	//TODO-Display notification
	void ApplyNeedStageEffects()
	{
		RemoveNeedEffects();
		auto player = RE::PlayerCharacter::GetSingleton();
		float stage = CurrentNeedStage->value;
		
		if (stage == 0) {
			player->AddSpell(NeedSpell0);
		} else if (stage == 1) {
			player->AddSpell(NeedSpell1);		
		} else if (stage == 2) {
			player->AddSpell(NeedSpell2);
		} else if (stage == 3) {
			player->AddSpell(NeedSpell3);
		} else if (stage == 4) {
			player->AddSpell(NeedSpell4);
		} else if (stage == 5) {
			player->AddSpell(NeedSpell5);
		}

	}

	/// <summary>
	/// Remove all need effects
	/// </summary>
	void RemoveNeedEffects()
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
	/// Get delta between time stamps in game seconds.
	/// </summary>
	/// <returns>Number of ticks that have passed since last update</returns>
	int GetGameTimeTicks()
	{
		int ticks = 0;

		auto currentTimeSeconds = RE::Calendar::GetSingleton()->GetCurrentGameTime() * 86400;
		auto lastTimeSeconds = LastUpdateTimeStamp->value;
		if (lastTimeSeconds <= 0) {
			//First update if timestamp not set. Return 0 delta
			return ticks;
		}

		ticks = int((currentTimeSeconds - lastTimeSeconds)) * int((1.0f / NeedRate->value));

		logger::debug("Incrementing need by ticks: " + ticks);

		return ticks;
	}

	//Apply Effect, Apply SFX, Apply Rumble
	
};
