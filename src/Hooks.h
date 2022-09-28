#pragma once

#include "SurvivalMode.h"

namespace Hooks
{
	inline static REL::Relocation<std::uintptr_t> On_Update_Hook{ REL::RelocationID(35565, 36564), REL::Relocate(0x1E, 0x6E) };

	inline void Install() {
		SurvivalMode::InstallUpdateHook();
	}
}
