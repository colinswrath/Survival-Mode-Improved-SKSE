#pragma once
#include "RE/A/Actor.h"
#include "RE/B/BSTEvent.h"
#include "RE/T/TESFastTravelEndEvent.h"
#include "REX/W32/KERNEL32.h"

namespace SurvivalModeVR {
	enum APIVersion {
		API_V1 = 1,
	};

	struct API1 {
		typedef float (*pfnGetActorWarmthRating)(RE::Actor *actor);
		pfnGetActorWarmthRating GetActorWarmthRating;

		typedef RE::BSTEventSource<RE::TESFastTravelEndEvent> *(*pfnGetFastTravelEndEventSource)();
		pfnGetFastTravelEndEventSource GetFastTravelEndEventSource;
	};

	inline void *GetAPI(APIVersion apiVersion) {
		auto pluginHandle = REX::W32::GetModuleHandleA("SurvivalModeVR.dll");
		if (!pluginHandle) return nullptr;
		typedef void * (*GetAPIFunc)(APIVersion);
		auto getAPIFunc = (GetAPIFunc) REX::W32::GetProcAddress(pluginHandle, "GetPluginAPI");
		if (getAPIFunc) return getAPIFunc(apiVersion);
		return nullptr;
	}

	inline API1 *GetAPI1() { return (API1 *) GetAPI(API_V1); }
}
