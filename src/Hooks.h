#pragma once

#include "SurvivalMode.h"

namespace Hooks
{
	inline static REL::Relocation<std::uintptr_t> On_Update_Hook{ REL::RelocationID(35565, 36564), REL::Relocate(0x748, 0xC26) };
	inline static REL::Relocation<std::uintptr_t> Fast_Travel_Message_Hook { REL::RelocationID(39372, 40444), REL::Relocate(0x176, 0x18B) };


	inline void Install() {
		SurvivalMode::InstallUpdateHook();
		SurvivalMode::InstallFtMessageHook();
	}
}
