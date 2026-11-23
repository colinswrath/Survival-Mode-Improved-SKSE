#include "Hooks.h"
#include "Events.h"
#include "Settings/FormLoader.h"
#include "Serialization.h"
#include "Settings/Settings.h"
#include "PapyrusAPI/PapyrusAPI.h"
#include "AvPenaltyManager.h"
#include "SMI_API.h"
#include "SMI_ModAPI.h"
#include "Utility/GlobalValueUpdater.h"

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostLoad:
		if (REL::Module::IsVR()) {
			Utility::GetSingleton()->InitVRAPI();
		}
		break;
	case SKSE::MessagingInterface::kNewGame:
		Serialization::LoadChecks();
        Utility::GetSingleton()->ClearSurvivalModeQuestScripts();
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		Serialization::LoadChecks();
        Utility::GetSingleton()->ClearSurvivalModeQuestScripts();
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		FormLoader::GetSingleton()->LoadAllForms();
        GlobalUpdater::LoadBindings();
        GlobalUpdater::CacheGlobals();
		Settings::LoadSettings();
        Events::Register();
        AvPenaltyManager::GetSingleton()->InitializeHandlers();
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    Init(skse);

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };
    logger::init();

    logger::info("{} {} loading...", plugin->GetName(), version);

    FormLoader::GetSingleton()->CacheGameAddresses();
    SKSE::AllocTrampoline(42);
    Hooks::Install();

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

extern "C" DLLEXPORT void* SKSEAPI RequestPluginAPI(const SMI_API::InterfaceVersion a_interfaceVersion)
{
    auto api = Messaging::SmiInterface::GetSingleton();

    logger::info("SMI_API::RequestPluginAPI called, InterfaceVersion {}", static_cast<uint8_t>(a_interfaceVersion));

    switch (a_interfaceVersion) {
    case SMI_API::InterfaceVersion::V1:
        logger::info("SMI_API::RequestPluginAPI returned the API singleton");
        return static_cast<void*>(api);
    }

    logger::info("SMI_API::RequestPluginAPI requested the wrong interface version");
    return nullptr;
}
