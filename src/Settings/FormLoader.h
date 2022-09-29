#pragma once

#include "Needs/NeedHunger.h"
#include "PlayerStatus.h"

class FormLoader
{
public:

	static FormLoader* GetSingleton()
	{
		static FormLoader formLoader;
		return &formLoader;
	}

	void LoadSettings();

	void LoadAllForms() 
	{
		LoadHungerForms();
		LoadFatigueForms();
		LoadColdForms();
		LoadMiscForms();
	}

	void LoadHungerForms()
	{
		if (!RE::TESDataHandler::GetSingleton()->LookupLoadedLightModByName("ccQDRSSE001-SurvivalMode.esl"))
			return;

		auto hungerSystem = NeedHunger::GetSingleton();

		hungerSystem->NeedSpell0 = RE::TESForm::LookupByEditorID("Survival_HungerStage0")->As<RE::SpellItem>();
		hungerSystem->NeedSpell1 = RE::TESForm::LookupByEditorID("Survival_HungerStage1")->As<RE::SpellItem>();
		hungerSystem->NeedSpell2 = RE::TESForm::LookupByEditorID("Survival_HungerStage2")->As<RE::SpellItem>();
		hungerSystem->NeedSpell3 = RE::TESForm::LookupByEditorID("Survival_HungerStage3")->As<RE::SpellItem>();
		hungerSystem->NeedSpell4 = RE::TESForm::LookupByEditorID("Survival_HungerStage4")->As<RE::SpellItem>();
		hungerSystem->NeedSpell5 = RE::TESForm::LookupByEditorID("Survival_HungerStage5")->As<RE::SpellItem>();

		hungerSystem->NeedStage1 = RE::TESForm::LookupByEditorID("Survival_HungerStage1Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage2 = RE::TESForm::LookupByEditorID("Survival_HungerStage2Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage3 = RE::TESForm::LookupByEditorID("Survival_HungerStage3Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage4 = RE::TESForm::LookupByEditorID("Survival_HungerStage4Value")->As<RE::TESGlobal>();
		hungerSystem->NeedStage5 = RE::TESForm::LookupByEditorID("Survival_HungerStage5Value")->As<RE::TESGlobal>();
		hungerSystem->NeedMaxValue = RE::TESForm::LookupByEditorID("Survival_HungerNeedMaxValue")->As<RE::TESGlobal>();

		hungerSystem->NeedMessage0 = RE::TESForm::LookupByEditorID("Survival_Hunger0Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage1 = RE::TESForm::LookupByEditorID("Survival_Hunger1Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage1Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger1MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage2 = RE::TESForm::LookupByEditorID("Survival_Hunger2Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage2Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger2MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage3 = RE::TESForm::LookupByEditorID("Survival_Hunger3Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage3Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger3MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage4 = RE::TESForm::LookupByEditorID("Survival_Hunger4Message")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage4Decreasing = RE::TESForm::LookupByEditorID("Survival_Hunger4MessageDecreasing")->As<RE::BGSMessage>();
		hungerSystem->NeedMessage5 = RE::TESForm::LookupByEditorID("Survival_Hunger5Message")->As<RE::BGSMessage>();

		hungerSystem->NeedRate = RE::TESForm::LookupByEditorID("Survival_HungerNeedRate")->As<RE::TESGlobal>();		//TODO- Account for esp balance per /hr instead of /min
		hungerSystem->CurrentNeedStage = RE::TESForm::LookupByEditorID("SMI_CurrentHungerStage")->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedValue = RE::TESForm::LookupByEditorID("Survival_HungerNeedValue")->As<RE::TESGlobal>();

		hungerSystem->LastUpdateTimeStamp = RE::TESForm::LookupByEditorID("SMI_HungerLastUpdateTimeStamp")->As<RE::TESGlobal>();
		hungerSystem->NeedSleepRateMult = RE::TESForm::LookupByEditorID("Survival_NeedSleepReducedMetabolismMult")->As<RE::TESGlobal>();

		logger::info("All forms are loaded.");
	}

	void LoadFatigueForms()
	{

	}

	void LoadColdForms()
	{

	}

	void LoadMiscForms()
	{

		 //= RE::TESForm::LookupByEditorID("Survival_OblivionAreas")->As<RE::BGSListForm>();

	}
};
