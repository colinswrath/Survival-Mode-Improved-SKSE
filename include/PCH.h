#pragma once

/* +++++++++++++++++++++++++ C++23 Standard Library +++++++++++++++++++++++++ */

// Concepts library
#include <concepts>

// Utilities library
#include <any>
#include <bitset>
#include <chrono>
#include <compare>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <expected>
#include <functional>
#include <initializer_list>
#include <optional>
#include <source_location>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <variant>
#include <version>

// Dynamic memory management
#include <memory>
#include <memory_resource>
#include <new>
#include <scoped_allocator>

// Numeric limits
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <limits>
#include <stdfloat>

// Error handling
#include <cassert>
#include <cerrno>
#include <exception>
#include <stacktrace>
#include <stdexcept>
#include <system_error>

// Strings library
#include <cctype>
#include <charconv>
#include <cstring>
#include <cuchar>
#include <cwchar>
#include <cwctype>
#include <string>
#include <string_view>

// Containers library
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <span>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Iterators library
#include <iterator>

// Ranges library
#include <ranges>

// Algorithms library
#include <algorithm>
#include <execution>

// Numerics library
#include <bit>
#include <cfenv>
#include <cmath>
#include <complex>
#include <numbers>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>

// Localization library
#include <clocale>
#include <locale>

// Input/output library
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <print>
#include <spanstream>
#include <sstream>
#include <streambuf>
#include <strstream>
#include <syncstream>

// Filesystem library
#include <filesystem>

// Regular Expressions library
#include <regex>

// Atomic Operations library
#include <atomic>

// Thread support library
#include <barrier>
#include <condition_variable>
#include <future>
#include <latch>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <stop_token>
#include <thread>

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// clang-format off
#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <ShlObj_core.h>
#include <Psapi.h>
#include <Windows.h>
// clang-format on

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

// ClibUtil
#include <CLibUtil/distribution.hpp>
#include <CLibUtil/numeric.hpp>
#include <CLibUtil/rng.hpp>
#include <CLibUtil/string.hpp>
#include <ClibUtil/simpleINI.hpp>
#include <ClibUtil/singleton.hpp>

#include <ClibUtil/editorID.hpp>

#define DLLEXPORT __declspec(dllexport)

#include "Version.h"

using namespace std::literals;
using namespace REL::literals;

namespace logger = SKSE::log;

template <typename T>
class Singleton
{
protected:
    constexpr Singleton() noexcept  = default;
    constexpr ~Singleton() noexcept = default;

public:
    constexpr Singleton(const Singleton&)      = delete;
    constexpr Singleton(Singleton&&)           = delete;
    constexpr auto operator=(const Singleton&) = delete;
    constexpr auto operator=(Singleton&&)      = delete;

    [[nodiscard]] static constexpr T* GetSingleton() noexcept
    {
        static T singleton;
        return std::addressof(singleton);
    }
};

template <typename TDerived, typename TEvent>
class EventSingleton : public RE::BSTEventSink<TEvent>
{
protected:
    constexpr EventSingleton() noexcept           = default;
    constexpr ~EventSingleton() noexcept override = default;

public:
    constexpr EventSingleton(const EventSingleton&) = delete;
    constexpr EventSingleton(EventSingleton&&)      = delete;
    constexpr auto operator=(const EventSingleton&) = delete;
    constexpr auto operator=(EventSingleton&&)      = delete;

    [[nodiscard]] static constexpr TDerived* GetSingleton() noexcept
    {
        static TDerived singleton;
        return std::addressof(singleton);
    }

    static constexpr void Register() noexcept
    {
        using TEventSource = RE::BSTEventSource<TEvent>;

        constexpr std::string dirty_name{ typeid(TEvent).name() };
        const std::regex      p{ "struct |RE::|SKSE::| * __ptr64" };
        const auto            name{ std::regex_replace(dirty_name, p, "") };

        if constexpr (std::is_base_of_v<TEventSource, RE::BSInputDeviceManager>) {
            const auto manager{ RE::BSInputDeviceManager::GetSingleton() };
            manager->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_base_of_v<TEventSource, RE::UI>) {
            const auto ui{ RE::UI::GetSingleton() };
            ui->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::ActionEvent>) {
            SKSE::GetActionEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::CameraEvent>) {
            SKSE::GetCameraEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::CrosshairRefEvent>) {
            SKSE::GetCrosshairRefEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::ModCallbackEvent>) {
            SKSE::GetModCallbackEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::NiNodeUpdateEvent>) {
            SKSE::GetNiNodeUpdateEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_base_of_v<TEventSource, RE::ScriptEventSourceHolder>) {
            const auto holder{ RE::ScriptEventSourceHolder::GetSingleton() };
            holder->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
        SKSE::stl::report_and_fail(fmt::format("{}: Failed to register {} handler", plugin->GetName(), name));
    }
};

namespace stl
{
    using namespace SKSE::stl;

    template <typename T>
    constexpr auto write_thunk_call() noexcept
    {
        SKSE::AllocTrampoline(14);
        auto& trampoline{ SKSE::GetTrampoline() };
        T::func = trampoline.write_call<5>(T::address, T::Thunk);
    }

    template <typename TDest, typename TSource>
    constexpr auto write_vfunc() noexcept
    {
        REL::Relocation<std::uintptr_t> vtbl{ TDest::VTABLE[0] };
        TSource::func = vtbl.write_vfunc(TSource::idx, TSource::Thunk);
    }

    template <typename T>
    constexpr auto write_vfunc(const REL::VariantID variant_id) noexcept
    {
        REL::Relocation<std::uintptr_t> vtbl{ variant_id };
        T::func = vtbl.write_vfunc(T::idx, T::Thunk);
    }
} // namespace stl
