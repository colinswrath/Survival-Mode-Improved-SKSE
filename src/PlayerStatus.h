#pragma once


class PlayerStatus
{
public:

	RE::TESGlobal* Survival_ModeToggle;
	RE::TESGlobal* Survival_ModeEnabled;
	RE::TESGlobal* Survival_ModeEnabledShared;
	RE::TESGlobal* Survival_ModeCanBeEnabled;

	RE::BGSListForm* Survival_OblivionCells;
	RE::BGSListForm* Survival_OblivionLocations;
	RE::BGSListForm* Survival_OblivionAreas;

	static PlayerStatus* GetSingleton()
	{
		static PlayerStatus playerStatus;
		return &playerStatus;
	}

	inline static bool IsSurvivalEnabled()
	{
		return (bool)PlayerStatus::GetSingleton()->Survival_ModeEnabled->value;
	}

	inline static bool SurvivalToggle()
	{
		return (bool)PlayerStatus::GetSingleton()->Survival_ModeToggle->value;
	}
};
