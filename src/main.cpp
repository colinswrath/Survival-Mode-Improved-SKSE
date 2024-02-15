#include "Hooks.h"
#include "Events.h"
#include "Settings/FormLoader.h"
#include "Serialization.h"
#include "Settings/Settings.h"
#include "PapyrusAPI/PapyrusAPI.h"

void InitLogger()
{
    auto path{ SKSE::log::log_directory() };
    if (!path)
        stl::report_and_fail("Unable to lookup SKSE logs directory.");
    *path /= SKSE::PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;
    if (IsDebuggerPresent())
        log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
    else
        log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));

    log->set_level(spdlog::level::level_enum::info);
    log->flush_on(spdlog::level::level_enum::trace);

    set_default_logger(std::move(log));

    spdlog::set_pattern("[%T.%e UTC%z] [%L] [%=5t] %v");
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

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitLogger();

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} loading...", plugin->GetName(), version);

    #ifdef SKYRIM_SUPPORT_AE
        logger::info("Post 1130 build is active.");
    #endif // SKYRIM_SUPPORT_AE

    SKSE::Init(skse);

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
    }
    else {
        SKSE::stl::report_and_fail("SMI-SKSE: Failure to register Papyrus bindings.");
    }

    logger::info("SMI loaded successfully");
    return true;
}
