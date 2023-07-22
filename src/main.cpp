#include "Hooks.h"
#include "Events.h"
#include "Settings/FormLoader.h"
#include "Serialization.h"
#include "Settings/Settings.h"
#include "PapyrusAPI/PapyrusAPI.h"

void InitLogger()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= std::format("{}.log"sv, Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

//	1.5.97 Necessary
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface * a_skse, SKSE::PluginInfo * a_info)
{
	InitLogger();

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical(FMT_STRING("Loaded in editor, marking as incompatible"));
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_VR_1_4_15) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kNewGame:
		Serialization::LoadChecks();
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		Serialization::LoadChecks();
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		FormLoader::GetSingleton()->LoadAllForms();
		Settings::LoadSettings();
		break;
	}
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version =
[]() {
	SKSE::PluginVersionData v{};
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("colinswrath"sv);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse(true);
	v.UsesStructsPost629(false);
	return v;
}();


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitLogger();
	logger::info("loading SMI");

	SKSE::Init(a_skse);
	FormLoader::GetSingleton()->CacheGameAddresses();
	SKSE::AllocTrampoline(42);
	Hooks::Install();
	Events::Register();

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(InitListener)) {
		return false;
	}

	if (auto serialization = SKSE::GetSerializationInterface()) {
		serialization->SetUniqueID(Serialization::ID);
		serialization->SetSaveCallback(&Serialization::SaveCallback);
		serialization->SetLoadCallback(&Serialization::LoadCallback);
		serialization->SetRevertCallback(&Serialization::RevertCallback);
	}

	if (SKSE::GetPapyrusInterface()->Register(PapyrusAPI::Register)) {
		logger::info("Papyrus functions bound.");
	} else {
		SKSE::stl::report_and_fail("SMI-SKSE: Failure to register Papyrus bindings.");
	}
	
	return true;
}
