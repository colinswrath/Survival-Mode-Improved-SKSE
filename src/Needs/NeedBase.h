#pragma once

class NeedBase
{
public:
	RE::TESGlobal* NeedRate;
	RE::TESGlobal* CurrentNeedStage;
	RE::TESGlobal* CurrentNeedValue;

	RE::TESGlobal* LastUpdateTimeStamp;

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

	/// <summary>
	/// Update function
	/// </summary>
	void OnUpdateNeed()
	{
		//int ticks = GetGameTimeTicks();
		GetGameTimeTicks();

		//if (ticks > 0) {
		//	IncrementNeed(ticks);
		//	SetNeedStage();
		//}	
	}

	/// <summary>
	/// Increment the need value based on the delta and need rate
	/// </summary>
	void IncrementNeed(int ticks)
	{	

		float incAmount = GetNeedIncrementAmount(ticks);

		float newNeedLevel = CurrentNeedValue->value + incAmount;

		if (newNeedLevel > NeedMaxValue->value) {
			newNeedLevel = NeedMaxValue->value;
		}

		CurrentNeedValue->value = newNeedLevel;
	}

	/// <summary>
	/// Determine the current need stage.
	/// If we are in a new stage then update the effects 
	/// </summary>
	void SetNeedStage()
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
	/// Get delta between time stamps in game seconds. For now, 1 tick = 1 in game minute
	/// </summary>
	/// <returns>Number of ticks that have passed since last update</returns>
	int GetGameTimeTicks()
	{
		int ticks = 0;

		auto currentTimeMinutes = RE::Calendar::GetSingleton()->GetCurrentGameTime() * 1440;
		logger::info("Current time: " + std::to_string(currentTimeMinutes));

		if (!LastUpdateTimeStamp) {
			logger::info("Last update NULL");
		}

		auto lastTimeMinutes = LastUpdateTimeStamp->value;
		if (lastTimeMinutes <= 0) {
			//First update if timestamp not set. Return 0 delta
			LastUpdateTimeStamp->value = currentTimeMinutes;
			return ticks;
		}

		logger::info("Last time: " + std::to_string(LastUpdateTimeStamp->value));


		ticks = int((currentTimeMinutes - lastTimeMinutes)) * int((1.0f / NeedRate->value));

		logger::info("Current ticks: " + ticks);

		//If at least one tick has occured then set the timeStamp. Otherwise, wait for a tick
		if (ticks > 0) {
			logger::info("Incrementing need by ticks: " + ticks);
			LastUpdateTimeStamp->value = currentTimeMinutes;
		}

		return ticks;
	}

	//TODO-Apply Effect, Apply SFX, Apply Rumble
	
};
