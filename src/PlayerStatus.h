#pragma once

RE::BGSListForm* Survival_OblivionCells;
RE::BGSListForm* Survival_OblivionLocations;
RE::BGSListForm* Survival_OblivionAreas;

class PlayerStatus
{
	static PlayerStatus* GetSingleton()
	{
		static PlayerStatus playerStatus;
		return &playerStatus;
	}

	
};
