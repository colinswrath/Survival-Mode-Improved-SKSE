#pragma once

static float& g_deltaTime = (*(float*)RELOCATION_ID(523660, 410199).address());
static float lastTime;

class SurvivalMode
{
public:
	static bool InstallUpdateHook();
	inline static std::int32_t OnUpdate(std::int64_t a1);
	inline static REL::Relocation<decltype(OnUpdate)> _OnUpdate;
};
