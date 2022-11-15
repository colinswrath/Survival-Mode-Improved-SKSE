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

	
	/// <summary>
	/// Checks to run when reloading a save or new game
	/// </summary>
	inline void LoadChecks()
	{
		auto exhaustion = NeedExhaustion::GetSingleton();

		auto util = Utility::GetSingleton();

		if (util->AutoStart) {
			if (util->Survival_ModeCanBeEnabled->value == 0.0f) {  //This will only be 0 in the event you havent started SMI yet
				util->Survival_ModeToggle->value = 1.0f;
			}
		}

		if (!util->HelpManualPC->HasForm(util->Survival_HelpSurvivalModeLong)) {
			util->HelpManualPC->AddForm(util->Survival_HelpSurvivalModeLong);
		}

		if (!util->HelpManualXBox->HasForm(util->Survival_HelpSurvivalModeLongXbox)) {
			util->HelpManualXBox->AddForm(util->Survival_HelpSurvivalModeLongXbox);
		}

		if (!exhaustion->CurrentlyStopped) {
			exhaustion->PlayerSleepQuest->Stop();
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

		if (type != SerializationType) {
			return;
		}
			
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
}
