#include "SurvivalMode.h"
#include "Hooks.h"
#include "Needs/NeedHunger.h"

std::int32_t SurvivalMode::OnUpdate(std::int64_t a1)
{
	if (!RE::UI::GetSingleton()->GameIsPaused()) {
		
		if (g_deltaTime > 0) {
			lastTime += g_deltaTime;
			if (lastTime >= 1.0f) {
				logger::info("Update at: " + std::to_string(RE::Calendar::GetSingleton()->GetCurrentGameTime() * 86400) + " game time in seconds");
				lastTime = 0;
			}
		}
	}

	return _OnUpdate(a1);
}

bool SurvivalMode::InstallUpdateHook()
{
	auto& trampoline = SKSE::GetTrampoline();
	_OnUpdate = trampoline.write_call<5>(Hooks::On_Update_Hook.address(), OnUpdate);
	logger::info("Installed update hook");
	return true;
}

