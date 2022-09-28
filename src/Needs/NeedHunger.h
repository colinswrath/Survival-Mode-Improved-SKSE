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

};
