#pragma once

#include "SurvivalMode.h"

namespace Hooks
{
    inline static REL::Relocation<std::uintptr_t> On_Update_Hook{ REL::RelocationID(35565, 36564), REL::Relocate(0x748, 0xC38, 0x7D7) }; // 1170 is 0xC26, not sure what to do yet
    inline static REL::Relocation<std::uintptr_t> On_Update_Hook_Pre17{ REL::RelocationID(35565, 36564), REL::Relocate(0x748, 0xC26, 0x7D7) };
	inline static REL::Relocation<std::uintptr_t> Fast_Travel_Message_Hook { REL::RelocationID(39372, 40444), REL::Relocate(0x176, 0x18B) };
	inline static REL::Relocation<std::uintptr_t> Fast_Travel_Message_Hook_VR { REL::Offset(0x9162A8) };


	inline void Install() {
		SurvivalMode::InstallUpdateHook();
		SurvivalMode::InstallFtMessageHook();
	}
}
