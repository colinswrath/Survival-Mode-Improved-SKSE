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

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\SurvivalModeImproved.ini)");

		const auto dataHandler = RE::TESDataHandler::GetSingleton();

		logger::info("Loading season mults");

		const char* currentHeader = settings->SosSeasonMultHeader;
		if (dataHandler->LookupLoadedModByName("SnowOverSkyrim.esp")) {
			currentHeader = settings->DefaultSeasonMultHeader;
		}
		
		auto cold = NeedCold::GetSingleton();

		logger::info("Loading warm season vals");		

		cold->SeasonWarmMults[0]=std::stof((ini.GetValue(currentHeader, "fMorningStarWarmMult", "")));
		cold->SeasonWarmMults[1]=std::stof((ini.GetValue(currentHeader, "fSunsDawnWarmMult", "")));
		cold->SeasonWarmMults[2]=std::stof((ini.GetValue(currentHeader, "fFirstSeedWarmMult", "")));
		cold->SeasonWarmMults[3]=std::stof((ini.GetValue(currentHeader, "fRainsHandWarmMult", "")));
		cold->SeasonWarmMults[4]=std::stof((ini.GetValue(currentHeader, "fSecondSeedWarmMult", "")));
		cold->SeasonWarmMults[5]=std::stof((ini.GetValue(currentHeader, "fMidYearWarmMult", "")));
		cold->SeasonWarmMults[6]=std::stof((ini.GetValue(currentHeader, "fSunsHeightWarmMult", "")));
		cold->SeasonWarmMults[7]=std::stof((ini.GetValue(currentHeader, "fLastSeedWarmMult", "")));
		cold->SeasonWarmMults[8]=std::stof((ini.GetValue(currentHeader, "fHearthfireWarmMult", "")));
		cold->SeasonWarmMults[9]=std::stof((ini.GetValue(currentHeader, "fFrostFallWarmMult", "")));
		cold->SeasonWarmMults[10]=std::stof((ini.GetValue(currentHeader, "fSunsDuskWarmMult", "")));
		cold->SeasonWarmMults[11]=std::stof((ini.GetValue(currentHeader, "fEveningStarWarmMult", "")));

		logger::info("Loading cool season vals");		
		cold->SeasonCoolMults[0] = std::stof((ini.GetValue(currentHeader, "fMorningStarCoolMult", "1.5")));
		cold->SeasonCoolMults[1] = std::stof((ini.GetValue(currentHeader, "fSunsDawnCoolMult", "1.4")));
		cold->SeasonCoolMults[2] = std::stof((ini.GetValue(currentHeader, "fFirstSeedCoolMult", "1.3")));
		cold->SeasonCoolMults[3] = std::stof((ini.GetValue(currentHeader, "fRainsHandCoolMult", "1.2")));
		cold->SeasonCoolMults[4] = std::stof((ini.GetValue(currentHeader, "fSecondSeedCoolMult", "1.1")));
		cold->SeasonCoolMults[5] = std::stof((ini.GetValue(currentHeader, "fMidYearCoolMult", "1.0")));
		cold->SeasonCoolMults[6] = std::stof((ini.GetValue(currentHeader, "fSunsHeightCoolMult", "0.8")));
		cold->SeasonCoolMults[7] = std::stof((ini.GetValue(currentHeader, "fLastSeedCoolMult", "1.0")));
		cold->SeasonCoolMults[8] = std::stof((ini.GetValue(currentHeader, "fHearthfireCoolMult", "1.3")));
		cold->SeasonCoolMults[9] = std::stof((ini.GetValue(currentHeader, "fFrostFallCoolMult", "1.4")));
		cold->SeasonCoolMults[10] = std::stof((ini.GetValue(currentHeader, "fSunsDuskCoolMult", "1.5")));
		cold->SeasonCoolMults[11] = std::stof((ini.GetValue(currentHeader, "fEveningStarCoolMult", "1.6")));

		logger::info("Loading reach season vals");		
		cold->SeasonReachMults[0] = std::stof((ini.GetValue(currentHeader, "fMorningStarReachMult", "")));
		cold->SeasonReachMults[1] = std::stof((ini.GetValue(currentHeader, "fSunsDawnReachMult", "")));
		cold->SeasonReachMults[2] = std::stof((ini.GetValue(currentHeader, "fFirstSeedReachMult", "")));
		cold->SeasonReachMults[3] = std::stof((ini.GetValue(currentHeader, "fRainsHandReachMult", "")));
		cold->SeasonReachMults[4] = std::stof((ini.GetValue(currentHeader, "fSecondSeedReachMult", "")));
		cold->SeasonReachMults[5] = std::stof((ini.GetValue(currentHeader, "fMidYearReachMult", "")));
		cold->SeasonReachMults[6] = std::stof((ini.GetValue(currentHeader, "fSunsHeightReachMult", "")));
		cold->SeasonReachMults[7] = std::stof((ini.GetValue(currentHeader, "fLastSeedReachMult", "")));
		cold->SeasonReachMults[8] = std::stof((ini.GetValue(currentHeader, "fHearthfireReachMult", "")));
		cold->SeasonReachMults[9] = std::stof((ini.GetValue(currentHeader, "fFrostFallReachMult", "")));
		cold->SeasonReachMults[10] = std::stof((ini.GetValue(currentHeader, "fSunsDuskReachMult", "")));
		cold->SeasonReachMults[11] = std::stof((ini.GetValue(currentHeader, "fEveningStarReachMult", "")));

		logger::info("Loading freezing season vals");		
		cold->SeasonFreezingMults[0] = std::stof((ini.GetValue(currentHeader, "fMorningStarFreezingMult", "")));
		cold->SeasonFreezingMults[1] = std::stof((ini.GetValue(currentHeader, "fSunsDawnFreezingMult", "")));
		cold->SeasonFreezingMults[2] = std::stof((ini.GetValue(currentHeader, "fFirstSeedFreezingMult", "")));
		cold->SeasonFreezingMults[3] = std::stof((ini.GetValue(currentHeader, "fRainsHandFreezingMult", "")));
		cold->SeasonFreezingMults[4] = std::stof((ini.GetValue(currentHeader, "fSecondSeedFreezingMult", "")));
		cold->SeasonFreezingMults[5] = std::stof((ini.GetValue(currentHeader, "fMidYearFreezingMult", "")));
		cold->SeasonFreezingMults[6] = std::stof((ini.GetValue(currentHeader, "fSunsHeightFreezingMult", "")));
		cold->SeasonFreezingMults[7] = std::stof((ini.GetValue(currentHeader, "fLastSeedFreezingMult", "")));
		cold->SeasonFreezingMults[8] = std::stof((ini.GetValue(currentHeader, "fHearthfireFreezingMult", "")));
		cold->SeasonFreezingMults[9] = std::stof((ini.GetValue(currentHeader, "fFrostFallFreezingMult", "")));
		cold->SeasonFreezingMults[10] = std::stof((ini.GetValue(currentHeader, "fSunsDuskFreezingMult", "")));
		cold->SeasonFreezingMults[11] = std::stof((ini.GetValue(currentHeader, "fEveningStarFreezingMult", "")));

		logger::info("Settings loaded");		
	}

};
