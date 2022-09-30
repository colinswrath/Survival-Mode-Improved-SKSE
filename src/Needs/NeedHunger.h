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
		logger::info("Updating need hunger");
		int ticks = GetGameTimeTicks();

		Updating = true;
		if (ticks > 0) {
			IncrementNeed(ticks);
			SetNeedStage(true);
			SetLastTimeStamp(GetCurrentGameTimeInMinutes());
		}

		Updating = false;
	}
};
