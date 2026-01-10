#pragma once

#include "Needs/NeedHunger.h"
#include "Needs/NeedCold.h"
#include "Needs/NeedExhaustion.h"
#include "Utility.h"

namespace Serialization
{
	static constexpr std::uint32_t SerializationVersion = 1;
	static constexpr std::uint32_t ID = 'SMIF';
	static constexpr std::uint32_t SerializationType = 'SMIV';
	void LoadPreLoadGlobalVals();
	
	/// <summary>
	/// Checks to run when reloading a save or new game
	/// </summary>
	inline void LoadChecks()
	{
		auto exhaustion = NeedExhaustion::GetSingleton();
		auto hunger = NeedHunger::GetSingleton();
		auto player = Utility::GetPlayer();
		auto util = Utility::GetSingleton();

        //Stop SM quests
        for(auto* quest: util->smQuestsToHandle)
        {
            quest->Stop();
        }

		if (util->AutoStart) {
			if (util->Survival_ModeCanBeEnabled->value == 0.0f) {  //This will only be 0 in the event you havent started SMI yet
				util->Survival_ModeToggle->value = 1.0f;
			}
		}

		//Overwrite globals so esp edits take effect mid save
		if (util->forceUpdateGlobalValues) {
			LoadPreLoadGlobalVals();
		}

		if (!util->HelpManualPC->HasForm(util->Survival_HelpSurvivalModeLong)) {
			util->HelpManualPC->AddForm(util->Survival_HelpSurvivalModeLong);
		}

		if (!util->HelpManualXBox->HasForm(util->Survival_HelpSurvivalModeLongXbox)) {
			util->HelpManualXBox->AddForm(util->Survival_HelpSurvivalModeLongXbox);
		}

		if (util->DisableCarryWeightPenalty) {
			if (player) {
				player->RemoveSpell(util->Survival_abLowerCarryWeightSpell);
			}
		}

		if (!player->HasSpell(util->Survival_abLowerRegenSpell)) {
			player->AddSpell(util->Survival_abLowerRegenSpell);
		}

		hunger->SetHungerFoodItemDesc();

		if (!exhaustion->CurrentlyStopped && util->SMI_ExhaustionShouldBeEnabled->value == 1.0) {
			exhaustion->PlayerSleepQuest->Stop();
		} else if (util->SMI_ExhaustionShouldBeEnabled->value != 1.0 && exhaustion->PlayerSleepQuest->IsStopped()) {
			exhaustion->PlayerSleepQuest->Start();	
		}
	}

	inline void SaveCallback(SKSE::SerializationInterface* a_skse)
	{
		auto hunger = NeedHunger::GetSingleton();
		auto fatigue = NeedExhaustion::GetSingleton();
		auto cold = NeedCold::GetSingleton();
		auto utility = Utility::GetSingleton();

		if (!a_skse->OpenRecord(SerializationType, SerializationVersion)) {
			logger::error("Failed to open damage values record");
			return;
		} else {

			std::vector<float> smiValues = {
				static_cast<float>(hunger->CurrentlyStopped),
				static_cast<float>(fatigue->CurrentlyStopped),
				static_cast<float>(cold->CurrentlyStopped),
				static_cast<float>(utility->WasInOblivion),
			};

			if (!a_skse->WriteRecordData(smiValues.size())) {
				logger::error("Failed to write size of record data");
				return;
			} else {
				for (auto& elem : smiValues) {
					if (!a_skse->WriteRecordData(elem)) {
						logger::error("Failed to write data for warmth elem");
						return;
					}
					logger::info(FMT_STRING("Serialized: {}"),std::to_string(elem));
				}
			}
		}
	}


	inline void LoadCallback(SKSE::SerializationInterface* a_skse)
	{
		auto hunger = NeedHunger::GetSingleton();
		auto fatigue = NeedExhaustion::GetSingleton();
		auto cold = NeedCold::GetSingleton();
		auto utility = Utility::GetSingleton();

		std::vector<float> smiValues;

		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		a_skse->GetNextRecordInfo(type, version, length);
			
		if(version != SerializationVersion)
		{
			logger::error("Unable to load data");
			return;
		}

		std::size_t size;
		if (!a_skse->ReadRecordData(size)) {
			logger::error("Failed to load size");
			return;
		}

		for (std::size_t i = 0; i < size; ++i) {
			float elem;
			if (!a_skse->ReadRecordData(elem)) {
				logger::error("Failed to load setting element");
				return;
			} else {
				logger::info(FMT_STRING("Deserialized: {}"),std::to_string(elem));
				smiValues.push_back(elem);
			}
		}

		hunger->CurrentlyStopped = smiValues[0];
		fatigue->CurrentlyStopped = smiValues[1];
		cold->CurrentlyStopped = smiValues[2];
		utility->WasInOblivion = smiValues[3];
	}

	inline void RevertCallback([[maybe_unused]] SKSE::SerializationInterface* a_skse)
	{
		auto hunger = NeedHunger::GetSingleton();
		auto fatigue = NeedExhaustion::GetSingleton();
		auto cold = NeedCold::GetSingleton();
		auto utility = Utility::GetSingleton();

		hunger->CurrentlyStopped = false;
		fatigue->CurrentlyStopped = false;
		cold->CurrentlyStopped = false;
		utility->WasInOblivion = false;
	}

	void LoadPreLoadGlobalVals()
	{
		logger::info("Load global values for updating");

		auto util = Utility::GetSingleton();
		auto cold = NeedCold::GetSingleton();
		auto exhaustion = NeedExhaustion::GetSingleton();
		auto hunger = NeedHunger::GetSingleton();

		//Cold
		cold->NeedStage1->value = util->LoadColdStage1Val;
		cold->NeedStage2->value = util->LoadColdStage2Val;
		cold->NeedStage3->value = util->LoadColdStage3Val;
		cold->NeedStage4->value = util->LoadColdStage4Val;
		cold->NeedStage5->value = util->LoadColdStage5Val;
		util->SMI_ColdShouldBeEnabled->value = util->coldShouldBeEnabled;
		cold->NeedAvPenDisabled->value = util->coldAVPenDisabled;
		cold->Survival_ColdResistMaxValue->value = util->coldResistMaxValue;
		cold->Survival_AfflictionColdChance->value = util->coldAfflictionChance;

		//Exhaustion
		exhaustion->NeedStage1->value = util->LoadExhaustionStage1Val;
		exhaustion->NeedStage2->value = util->LoadExhaustionStage2Val;
		exhaustion->NeedStage3->value = util->LoadExhaustionStage3Val;
		exhaustion->NeedStage4->value = util->LoadExhaustionStage4Val;
		exhaustion->NeedStage5->value = util->LoadExhaustionStage5Val;
		util->SMI_ExhaustionShouldBeEnabled->value = util->exhaustionShouldBeEnabled;
		exhaustion->NeedAvPenDisabled->value = util->exhaustionAVPenDisabled;
		exhaustion->Survival_AfflictionExhaustionChance->value = util->exhaustionAfflictionChance;

		//hunger
		hunger->NeedStage1->value = util->LoadHungerStage1Val;
		hunger->NeedStage2->value = util->LoadHungerStage2Val;
		hunger->NeedStage3->value = util->LoadHungerStage3Val;
		hunger->NeedStage4->value = util->LoadHungerStage4Val;
		hunger->NeedStage5->value = util->LoadHungerStage5Val;
		util->SMI_HungerShouldBeEnabled->value = util->hungerShouldBeEnabled;
		hunger->NeedAvPenDisabled->value = util->hungerAVPenDisabled;
		hunger->Survival_AfflictionHungerChance->value = util->hungerAfflictionChance;

		exhaustion->NeedMaxValue->value = util->exhaustionMaxValue;
		cold->NeedMaxValue->value = util->coldMaxValue;
		hunger->NeedMaxValue->value = util->hungerMaxValue;

		exhaustion->Survival_ExhaustionRestorePerHour->value = util->exhaustionRestorePerHour;
		cold->SMI_ColdRate->value = util->coldRate;
		hunger->NeedRate->value = util->hungerRate = hunger->NeedRate->value;
		exhaustion->NeedRate->value = util->exhaustionRate = exhaustion->NeedRate->value;
	}
}
