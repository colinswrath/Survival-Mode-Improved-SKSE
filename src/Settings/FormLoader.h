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
	}

	void LoadHungerForms()
	{
		auto hungerSystem = NeedHunger::GetSingleton();

		hungerSystem->NeedSpell0 = RE::TESForm::LookupByEditorID("")->As<RE::SpellItem>();
		hungerSystem->NeedSpell1 = RE::TESForm::LookupByEditorID("")->As<RE::SpellItem>();
		hungerSystem->NeedSpell2 = RE::TESForm::LookupByEditorID("")->As<RE::SpellItem>();
		hungerSystem->NeedSpell3 = RE::TESForm::LookupByEditorID("")->As<RE::SpellItem>();
		hungerSystem->NeedSpell4 = RE::TESForm::LookupByEditorID("")->As<RE::SpellItem>();
		hungerSystem->NeedSpell5 = RE::TESForm::LookupByEditorID("")->As<RE::SpellItem>();

		hungerSystem->NeedStage0 = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->NeedStage1 = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->NeedStage2 = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->NeedStage3 = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->NeedStage4 = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->NeedStage5 = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();


		hungerSystem->NeedRate = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedStage = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
		hungerSystem->CurrentNeedLevel = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();

		hungerSystem->LastUpdateTimeStamp = RE::TESForm::LookupByEditorID("")->As<RE::TESGlobal>();
	}
};
