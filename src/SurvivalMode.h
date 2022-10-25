#pragma once

static float& g_deltaTime = (*(float*)RELOCATION_ID(523660, 410199).address());
static float lastTime;

class SurvivalMode
{
public:
	inline static int count;
	static bool InstallUpdateHook();
	inline static std::int32_t OnUpdate(std::int64_t a1);
	inline static REL::Relocation<decltype(OnUpdate)> _OnUpdate;
	inline static void StartSurvivalMode();
	inline static void StopSurvivalMode();

protected:
	inline static void SurvivalModeLoopUpdate();
	inline static void SendAllNeedsUpdate();
	inline static void StopAllNeeds();
	inline static void ShowNotification(RE::BGSMessage* msg);
	inline static void AddPlayerSpellPerks();
	inline static void RemovePlayerSpellPerks();
	inline static bool CheckOblivionStatus();
	inline static bool CheckJailStatus();
};
