#pragma once
#pragma warning(push)

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"


#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)

#include "Version.h"
