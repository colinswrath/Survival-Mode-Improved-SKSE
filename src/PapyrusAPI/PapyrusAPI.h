namespace PapyrusAPI
{
	void RestoreColdLevel(RE::StaticFunctionTag*, float coldRestoreAmount)
	{
		auto cold = NeedCold::GetSingleton();
		if (!cold->CurrentlyStopped && coldRestoreAmount>=0) {
			cold->NeedBase::DecreaseNeed(coldRestoreAmount);
		}
	}

	void RestoreHungerLevel(RE::StaticFunctionTag*, float hungerRestoreAmount)
	{
		auto hunger = NeedHunger::GetSingleton();
		if (!hunger->CurrentlyStopped && hungerRestoreAmount >= 0) {
			hunger->DecreaseNeed(hungerRestoreAmount);
		}
	}

	void RestoreExhaustionLevel(RE::StaticFunctionTag*, float exhaustionRestoreAmount)
	{
		auto exhaustion = NeedExhaustion::GetSingleton();
		if (!exhaustion->CurrentlyStopped && exhaustionRestoreAmount >= 0) {
			exhaustion->DecreaseNeed(exhaustionRestoreAmount);
		}
	}

	//TODO-Add functions for getting current ambient temp?
	bool Register(RE::BSScript::IVirtualMachine* vm)
	{
		if (!vm) {
			logger::critical("couldn't get VM State"sv);
			return false;
		}

		constexpr std::string_view scriptname = "SurvivalModeImprovedApi"sv;

		vm->RegisterFunction("RestoreColdLevel"sv, scriptname, RestoreColdLevel);
		vm->RegisterFunction("RestoreHungerLevel"sv, scriptname, RestoreHungerLevel);
		vm->RegisterFunction("RestoreExhaustionLevel"sv, scriptname, RestoreExhaustionLevel);

		logger::info("Registered SurvivalModeImprovedApi script");
		return true;
	}
}
