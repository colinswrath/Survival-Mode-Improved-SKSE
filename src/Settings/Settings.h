#include <SimpleIni.h>
#include <sstream>

class Settings
{
public:
	const char* DefaultSeasonMultHeader = "Default Seasons Mults";
	const char* SosSeasonMultHeader = "Seasons of Skyrim Mults";

	static Settings* GetSingleton()
	{
		static Settings settings;
		return &settings;
	}

	static void LoadSettings()
	{
		logger::info("Loading settings");
		auto settings = Settings::GetSingleton();
		auto util = Utility::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\SurvivalModeImproved.ini)");
		const auto dataHandler = RE::TESDataHandler::GetSingleton();

		logger::info("Loading general settings");
		util->DisableFastTravel = ini.GetBoolValue("General", "bDisableFastTravel", true);
		util->AutoStart = ini.GetBoolValue("General", "bAutoEnableSMOnNewGame", true);
		util->DisableCarryWeightPenalty = ini.GetBoolValue("General", "bDisableCarryWeightPenalty", true);

		logger::info("Loading season mults");

		const char* currentHeader = settings->SosSeasonMultHeader;
		if (dataHandler->LookupLoadedModByName("SnowOverSkyrim.esp")) {
			currentHeader = settings->DefaultSeasonMultHeader;
		}
		
		auto cold = NeedCold::GetSingleton();

		logger::info("Loading warm season vals");		

		cold->SeasonWarmMults[0]=std::stof((ini.GetValue(currentHeader, "fMorningStarWarmMult", "1.0")));
		cold->SeasonWarmMults[1]=std::stof((ini.GetValue(currentHeader, "fSunsDawnWarmMult", "1.0")));
		cold->SeasonWarmMults[2]=std::stof((ini.GetValue(currentHeader, "fFirstSeedWarmMult", "1.0")));
		cold->SeasonWarmMults[3]=std::stof((ini.GetValue(currentHeader, "fRainsHandWarmMult", "1.0")));
		cold->SeasonWarmMults[4]=std::stof((ini.GetValue(currentHeader, "fSecondSeedWarmMult", "1.0")));
		cold->SeasonWarmMults[5]=std::stof((ini.GetValue(currentHeader, "fMidYearWarmMult", "1.0")));
		cold->SeasonWarmMults[6]=std::stof((ini.GetValue(currentHeader, "fSunsHeightWarmMult", "1.0")));
		cold->SeasonWarmMults[7]=std::stof((ini.GetValue(currentHeader, "fLastSeedWarmMult", "1.0")));
		cold->SeasonWarmMults[8]=std::stof((ini.GetValue(currentHeader, "fHearthfireWarmMult", "1.0")));
		cold->SeasonWarmMults[9]=std::stof((ini.GetValue(currentHeader, "fFrostFallWarmMult", "1.0")));
		cold->SeasonWarmMults[10]=std::stof((ini.GetValue(currentHeader, "fSunsDuskWarmMult", "1.0")));
		cold->SeasonWarmMults[11]=std::stof((ini.GetValue(currentHeader, "fEveningStarWarmMult", "1.0")));

		logger::info("Loading cool season vals");		
		cold->SeasonCoolMults[0] = std::stof((ini.GetValue(currentHeader, "fMorningStarCoolMult", "1.0")));
		cold->SeasonCoolMults[1] = std::stof((ini.GetValue(currentHeader, "fSunsDawnCoolMult", "1.0")));
		cold->SeasonCoolMults[2] = std::stof((ini.GetValue(currentHeader, "fFirstSeedCoolMult", "1.0")));
		cold->SeasonCoolMults[3] = std::stof((ini.GetValue(currentHeader, "fRainsHandCoolMult", "1.0")));
		cold->SeasonCoolMults[4] = std::stof((ini.GetValue(currentHeader, "fSecondSeedCoolMult", "1.0")));
		cold->SeasonCoolMults[5] = std::stof((ini.GetValue(currentHeader, "fMidYearCoolMult", "1.0")));
		cold->SeasonCoolMults[6] = std::stof((ini.GetValue(currentHeader, "fSunsHeightCoolMult", "1.0")));
		cold->SeasonCoolMults[7] = std::stof((ini.GetValue(currentHeader, "fLastSeedCoolMult", "1.0")));
		cold->SeasonCoolMults[8] = std::stof((ini.GetValue(currentHeader, "fHearthfireCoolMult", "1.0")));
		cold->SeasonCoolMults[9] = std::stof((ini.GetValue(currentHeader, "fFrostFallCoolMult", "1.0")));
		cold->SeasonCoolMults[10] = std::stof((ini.GetValue(currentHeader, "fSunsDuskCoolMult", "1.0")));
		cold->SeasonCoolMults[11] = std::stof((ini.GetValue(currentHeader, "fEveningStarCoolMult", "1.0")));

		logger::info("Loading reach season vals");		
		cold->SeasonReachMults[0] = std::stof((ini.GetValue(currentHeader, "fMorningStarReachMult", "1.0")));
		cold->SeasonReachMults[1] = std::stof((ini.GetValue(currentHeader, "fSunsDawnReachMult", "1.0")));
		cold->SeasonReachMults[2] = std::stof((ini.GetValue(currentHeader, "fFirstSeedReachMult", "1.0")));
		cold->SeasonReachMults[3] = std::stof((ini.GetValue(currentHeader, "fRainsHandReachMult", "1.0")));
		cold->SeasonReachMults[4] = std::stof((ini.GetValue(currentHeader, "fSecondSeedReachMult", "1.0")));
		cold->SeasonReachMults[5] = std::stof((ini.GetValue(currentHeader, "fMidYearReachMult", "1.0")));
		cold->SeasonReachMults[6] = std::stof((ini.GetValue(currentHeader, "fSunsHeightReachMult", "1.0")));
		cold->SeasonReachMults[7] = std::stof((ini.GetValue(currentHeader, "fLastSeedReachMult", "1.0")));
		cold->SeasonReachMults[8] = std::stof((ini.GetValue(currentHeader, "fHearthfireReachMult", "1.0")));
		cold->SeasonReachMults[9] = std::stof((ini.GetValue(currentHeader, "fFrostFallReachMult", "1.0")));
		cold->SeasonReachMults[10] = std::stof((ini.GetValue(currentHeader, "fSunsDuskReachMult", "1.0")));
		cold->SeasonReachMults[11] = std::stof((ini.GetValue(currentHeader, "fEveningStarReachMult", "1.0")));

		logger::info("Loading freezing season vals");		
		cold->SeasonFreezingMults[0] = std::stof((ini.GetValue(currentHeader, "fMorningStarFreezingMult", "1.0")));
		cold->SeasonFreezingMults[1] = std::stof((ini.GetValue(currentHeader, "fSunsDawnFreezingMult", "1.0")));
		cold->SeasonFreezingMults[2] = std::stof((ini.GetValue(currentHeader, "fFirstSeedFreezingMult", "1.0")));
		cold->SeasonFreezingMults[3] = std::stof((ini.GetValue(currentHeader, "fRainsHandFreezingMult", "1.0")));
		cold->SeasonFreezingMults[4] = std::stof((ini.GetValue(currentHeader, "fSecondSeedFreezingMult", "1.0")));
		cold->SeasonFreezingMults[5] = std::stof((ini.GetValue(currentHeader, "fMidYearFreezingMult", "1.0")));
		cold->SeasonFreezingMults[6] = std::stof((ini.GetValue(currentHeader, "fSunsHeightFreezingMult", "1.0")));
		cold->SeasonFreezingMults[7] = std::stof((ini.GetValue(currentHeader, "fLastSeedFreezingMult", "1.0")));
		cold->SeasonFreezingMults[8] = std::stof((ini.GetValue(currentHeader, "fHearthfireFreezingMult", "1.0")));
		cold->SeasonFreezingMults[9] = std::stof((ini.GetValue(currentHeader, "fFrostFallFreezingMult", "1.0")));
		cold->SeasonFreezingMults[10] = std::stof((ini.GetValue(currentHeader, "fSunsDuskFreezingMult", "1.0")));
		cold->SeasonFreezingMults[11] = std::stof((ini.GetValue(currentHeader, "fEveningStarFreezingMult", "1.0")));

		logger::info("Loading cold settings");
		cold->BlizzardWindspeedThreshold = std::stof((ini.GetValue("Cold Settings", "fBlizzardWindSpeedThreshold", "150.0")));
		cold->ColdToRestoreInWarmArea = std::stof((ini.GetValue("Cold Settings", "fColdToRestoreInWarmArea", "1.5")));
		cold->AmbientWarmthWidgetColdLevelThreshold = std::stof((ini.GetValue("Cold Settings", "fAmbientWarmthWidgetColdLevelThreshold", "200.0")));
		cold->MaxWarmthRatingBonusPerc = std::clamp(std::stof((ini.GetValue("Cold Settings", "fMaxWarmthRatingBonus", "0.8"))), 0.0f, 1.0f);

		logger::info("Settings loaded");		
	}
};
