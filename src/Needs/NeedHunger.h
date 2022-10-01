#pragma once

#include "NeedBase.h"

class NeedHunger: public NeedBase
{
public:

	static NeedHunger* GetSingleton()
	{
		static NeedHunger hungerSystem;
		return &hungerSystem;
	}

	void UpdateNeed() override
	{
		int ticks = GetGameTimeTicks();

		if (ticks > 0) {
			Updating = true;
			IncrementNeed(ticks);
			SetNeedStage(true);
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}

		Updating = false;
	}
};
