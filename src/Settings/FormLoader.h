#pragma once

#include "Needs/NeedHunger.h"

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


		hungerSystem->NeedRate = RE::TESForm::LookupByEditorID("Survival_HungerNeedRate")->As<RE::TESGlobal>();		//TODO- Account for esp balance per /hr instead of /min
		hungerSystem->CurrentNeedStage = RE::TESForm::LookupByEditorID("SMI_CurrentHungerStage")->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedValue = RE::TESForm::LookupByEditorID("Survival_HungerNeedValue")->As<RE::TESGlobal>();

		hungerSystem->LastUpdateTimeStamp = RE::TESForm::LookupByEditorID("SMI_HungerLastUpdateTimeStamp")->As<RE::TESGlobal>();
	}

	void LoadFatigueForms()
	{

	}

	void LoadColdForms()
	{

	}

	void LoadMiscForms()
	{

	}
};
