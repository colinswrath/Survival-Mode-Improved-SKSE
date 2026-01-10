#pragma once
#include <Windows.h>
#include <cstdint>

namespace SMI_API
{
    constexpr const wchar_t* SMIModuleName = L"SurvivalModeImproved.dll";

    enum class InterfaceVersion : uint8_t
    {
        V1 = 1
    };

    class IVSmi1
    {
    public:
        [[nodiscard]] virtual void SetInjuryHandlingEnabled(bool enabled) noexcept = 0;
    };

    typedef void* (*_RequestPluginAPI)(const InterfaceVersion interfaceVersion);

    /// <summary>
    /// Request the True HUD API interface.
    /// Recommended: Send your request during or after SKSEMessagingInterface::kMessage_PostLoad to make sure the dll has already been loaded
    /// </summary>
    /// <param name="a_interfaceVersion">The interface version to request</param>
    /// <returns>The pointer to the API singleton, or nullptr if request failed</returns>
    [[nodiscard]] inline void* RequestPluginAPI(const InterfaceVersion a_interfaceVersion = InterfaceVersion::V1)
    {
        auto pluginHandle       = GetModuleHandleW(SMIModuleName);
        if (!pluginHandle)
            return nullptr;

        _RequestPluginAPI requestAPIFunction = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
        if (requestAPIFunction) {
            return requestAPIFunction(a_interfaceVersion);
        }

        return nullptr;
    }
}
