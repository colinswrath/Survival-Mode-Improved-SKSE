#pragma once

static float& g_deltaTime = (*(float*)RELOCATION_ID(523660, 410199).address());
static float lastTime;

class SurvivalMode
{
public:
	static bool InstallUpdateHook();
	static bool InstallFtMessageHook();
	inline static void StartSurvivalMode();
	inline static void StopSurvivalMode();

protected:
	inline static std::int32_t OnUpdate();
	inline static REL::Relocation<decltype(OnUpdate)> _OnUpdate;

	inline static void OverwriteFastTravelMessage(const char* a_notification, const char* a_soundToPlay = 0, bool a_cancelIfAlreadyQueued = true);
	inline static REL::Relocation<decltype(OverwriteFastTravelMessage)> _OverwriteFastTravelMessage;

	inline static void SurvivalModeLoopUpdate();
	inline static void SendAllNeedsUpdate();
	inline static void SendHungerUpdate();
	inline static void SendColdUpdate();
	inline static void SendExhaustionUpdate();
	inline static void StopAllNeeds();
	inline static void ShowNotification(RE::BGSMessage* msg);
	inline static void AddPlayerSpellPerks();
	inline static void RemovePlayerSpellPerks();
	inline static bool CheckOblivionStatus();
	inline static bool CheckJailStatus();
};
