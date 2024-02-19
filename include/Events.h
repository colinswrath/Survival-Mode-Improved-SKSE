#pragma once
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"
#include "Needs/NeedCold.h"
#include "Utility.h"

namespace Events
{
	static inline float Hours;

	static void ProcessSleepStartEvent()
	{
		Hours = Utility::GetCalendar()->GetHoursPassed();
		auto hunger = NeedHunger::GetSingleton();
		auto exhaustion = NeedExhaustion::GetSingleton();
		auto cold = NeedCold::GetSingleton();

		if (!hunger->CurrentlyStopped) {
			hunger->WasSleeping = true;
		}

		if (!exhaustion->CurrentlyStopped) {
			exhaustion->PlayerSleepQuest->Stop();
			exhaustion->WasSleeping = true;
		}

		if (!cold->CurrentlyStopped) {
			cold->WasSleeping = true;
		}
	}

	static void ProcessFastTravelEndEvent()
	{
		auto hunger = NeedHunger::GetSingleton();
		auto exhaustion = NeedExhaustion::GetSingleton();
		auto cold = NeedCold::GetSingleton();

		if (!hunger->CurrentlyStopped) {
			hunger->FastTravelled = true;
		}

		if (!exhaustion->CurrentlyStopped) {
			exhaustion->FastTravelled = true;
		}

		if (!cold->CurrentlyStopped) {
			cold->FastTravelled = true;
		}
	}

	static void ProcessSleepStopEvent()
	{
		auto exhaustion = NeedExhaustion::GetSingleton();

		if (!exhaustion->CurrentlyStopped) {

			exhaustion->SetLastTimeStamp();

			Hours = Utility::GetCalendar()->GetHoursPassed() - Hours;
			exhaustion->DecreaseExhaustion(Hours);
		}
	}
	
	static void ProcessHungerOnEquipEvent(RE::AlchemyItem* food)
	{
		auto hunger = NeedHunger::GetSingleton();
		auto util = Utility::GetSingleton();
		auto player = Utility::GetPlayer();

		if (!hunger->CurrentlyStopped || util->forceEnableFoodPoisoning) {
			if (hunger->Survival_FoodRawMeat->HasForm(food) || food->HasKeyword(hunger->VendorItemFoodRaw)) {
				float diseaseResistMult = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kResistDisease);

				if (!hunger->Survival_FoodPoisoningImmuneRaces->HasForm(player->GetRace()) && (diseaseResistMult < 1) && !player->HasKeyword(hunger->Survival_DiseaseFoodPoisoningKeyword)) {
					float baseChance = 50.0f;
					float chance = baseChance * (1.0f - diseaseResistMult);
					float result = Utility::GetRandomFloat(0, 100);
					if (result <= chance) {
						Utility::DoCombatSpellApply(player, hunger->Survival_DiseaseFoodPoisoning, player);
					}
				}
			}
		}

		if (!hunger->CurrentlyStopped) {
			for (auto effect : food->effects) {
				if (hunger->Survival_FoodRestoreHungerVerySmall == effect->baseEffect) {
					hunger->DecreaseNeed(hunger->Survival_HungerRestoreVerySmallAmount->value);
				} else if (hunger->Survival_FoodRestoreHungerSmall == effect->baseEffect) {
					hunger->DecreaseNeed(hunger->Survival_HungerRestoreSmallAmount->value);
				} else if (hunger->Survival_FoodRestoreHungerMedium == effect->baseEffect) {
					hunger->DecreaseNeed(hunger->Survival_HungerRestoreMediumAmount->value);
				} else if (hunger->Survival_FoodRestoreHungerLarge == effect->baseEffect) {
					hunger->DecreaseNeed(hunger->Survival_HungerRestoreLargeAmount->value);		
				}
			}
		}
	}

	static void ProcessOnHitEvent(RE::Actor* hitCause)
	{
		auto race = hitCause->GetRace();
		auto util = Utility::GetSingleton();
		
		if (util->IsSurvivalEnabled() && race && !util->DisableDiseaseApplicator) {
			if (util->Survival_BrownRotCarryingRaces->HasForm(race)) {
				Utility::DoCombatSpellApply(Utility::GetPlayer(), util->Survival_DiseaseBrownRot, Utility::GetPlayer());
			} else if (util->Survival_GreensporeCarryingRaces->HasForm(race)) {
				Utility::DoCombatSpellApply(Utility::GetPlayer(), util->Survival_DiseaseGreenspore, Utility::GetPlayer());
			} else if (util->Survival_GutwormCarryingRaces->HasForm(race)) {
				Utility::DoCombatSpellApply(Utility::GetPlayer(), util->Survival_DiseaseGutworm, Utility::GetPlayer());
			} 
		}
	}

	static void ProcessMagicEffectApplyEvent(RE::EffectSetting* effect)
	{
		auto cold = NeedCold::GetSingleton();
		auto hunger = NeedHunger::GetSingleton();
		auto util = Utility::GetSingleton();
		
		if (!cold->CurrentlyStopped) {
			if (cold->Survival_FoodRestoreCold == effect) {
				cold->NeedBase::DecreaseNeed(cold->Survival_ColdRestoreMediumAmount->value, cold->NeedStage1->value);
			}
		}

		if (!hunger->CurrentlyStopped) {
			if (effect == util->WerewolfFeedRestoreHealth) {
				//WW feed
				hunger->DecreaseNeed(hunger->Survival_HungerRestoreMediumAmount->value);
			} else if (effect == util->DA11AbFortifyHealth) {
				//Cannibal feed
				hunger->DecreaseNeed(hunger->Survival_HungerRestoreMediumAmount->value);
			}
		}
	}

	class OnFastTravelEndEventHandler : public RE::BSTEventSink<RE::TESFastTravelEndEvent>
	{
	public:
		static OnFastTravelEndEventHandler* GetSingleton()
		{
			static OnFastTravelEndEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESFastTravelEndEvent* a_event, RE::BSTEventSource<RE::TESFastTravelEndEvent>*) override
		{
			if (!a_event) {
				return RE::BSEventNotifyControl::kContinue;
			}

			ProcessFastTravelEndEvent();

			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnFastTravelEndEventHandler::GetSingleton());
		}
	};

	class OnSleepStartEventHandler : public RE::BSTEventSink<RE::TESSleepStartEvent>
	{
	public:
		static OnSleepStartEventHandler* GetSingleton()
		{
			static OnSleepStartEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStartEvent* a_event, RE::BSTEventSource<RE::TESSleepStartEvent>*) override
		{
			if (!a_event) {
				return RE::BSEventNotifyControl::kContinue;
			}

			ProcessSleepStartEvent();

			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnSleepStartEventHandler::GetSingleton());
		}
	};

	class OnSleepStopEventHandler : public RE::BSTEventSink<RE::TESSleepStopEvent>
	{
	public:
		static OnSleepStopEventHandler* GetSingleton()
		{
			static OnSleepStopEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*) override
		{
			if (!a_event) {
				return RE::BSEventNotifyControl::kContinue;
			}

			ProcessSleepStopEvent();

			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnSleepStopEventHandler::GetSingleton());
		}
	};

	class OnEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent>
	{
	public:
		static OnEquipEventHandler* GetSingleton()
		{
			static OnEquipEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event,[[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override
		{
			if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef()) {
				return RE::BSEventNotifyControl::kContinue;
			}

			auto alchemyItem = RE::TESForm::LookupByID<RE::AlchemyItem>(a_event->baseObject);

			if (alchemyItem && alchemyItem->IsFood() && Utility::IsSurvivalEnabled()) {
				ProcessHungerOnEquipEvent(alchemyItem);
			}

			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnEquipEventHandler::GetSingleton());
		}
	};

	class OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
	{
	public:

		static OnHitEventHandler* GetSingleton()
		{
			static OnHitEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override
		{
			if (!a_event || !a_event->target || !a_event->target->IsPlayerRef() || !a_event->cause) {
				return RE::BSEventNotifyControl::kContinue;
			}

			auto source = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);
			if (source && source->IsMelee()) {
				auto causeActor = a_event->cause->As<RE::Actor>();
				if (causeActor) {
					ProcessOnHitEvent(causeActor);
				}
			}
			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnHitEventHandler::GetSingleton());
		}
	};

	class OnEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
	{
	public:

		static OnEffectApplyEventHandler* GetSingleton()
		{
			static OnEffectApplyEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override
		{
			if (!a_event || !a_event->target || !a_event->target->IsPlayerRef()) {
				return RE::BSEventNotifyControl::kContinue;
			}

			auto effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);
			
			if (!effect) {
				return RE::BSEventNotifyControl::kContinue;
			}

			ProcessMagicEffectApplyEvent(effect);
			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnEffectApplyEventHandler::GetSingleton());
		}
	};

	class OnMenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:

		static OnMenuOpenCloseEventHandler* GetSingleton()
		{
			static OnMenuOpenCloseEventHandler singleton;
			return &singleton;
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override
		{
			if (!a_event || !a_eventSource) {
				return RE::BSEventNotifyControl::kContinue;
			}

			if(a_event->menuName == RE::MapMenu::MENU_NAME && Utility::GetSingleton()->DisableFastTravel) {
				if (a_event->opening && Utility::IsFastTravelEnabled() && Utility::DisableFTCheck()) {
					Utility::EnableFastTravel(false);
				} else if (!a_event->opening) {
					Utility::EnableFastTravel(true);	
				}
			}
			
			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			if (const auto ui = Utility::GetUI()) {
				ui->AddEventSink<RE::MenuOpenCloseEvent>(OnMenuOpenCloseEventHandler::GetSingleton());
			}
		}
	};

	inline static void Register()
	{
		OnSleepStartEventHandler::Register();
		OnSleepStopEventHandler::Register();
		OnEquipEventHandler::Register();
		OnHitEventHandler::Register();
		OnEffectApplyEventHandler::Register();
		OnMenuOpenCloseEventHandler::Register();
		OnFastTravelEndEventHandler::Register();
	}
}
