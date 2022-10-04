#pragma once
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"

namespace Events
{
	static inline float Hours;

	static void ProcessSleepStartEvent()
	{
		Hours = RE::Calendar::GetSingleton()->GetHoursPassed();
		auto hunger = NeedHunger::GetSingleton();
		auto exhaustion = NeedExhaustion::GetSingleton();

		if (!hunger->CurrentlyStopped) {
			NeedHunger::GetSingleton()->WasSleeping = true;
		}

		if (!exhaustion->CurrentlyStopped) {
			NeedExhaustion::GetSingleton()->WasSleeping = true;
		}

		//TODO-Cold
	}

	static void ProcessSleepStopEvent()
	{
		auto exhaustion = NeedExhaustion::GetSingleton();
		if (!exhaustion->CurrentlyStopped) {
			exhaustion->SetLastTimeStamp();
			Hours = RE::Calendar::GetSingleton()->GetHoursPassed() - Hours;

			logger::info("Hours slept "+std::to_string(Hours));

			exhaustion->DecreaseExhaustion(Hours);
		}
	}
	
	//TODO-Add actual decrementing of needs
	static void ProcessHungerOnEquipEvent(RE::AlchemyItem* food)
	{
		auto hunger = NeedHunger::GetSingleton();

		if (!hunger->CurrentlyStopped) {
			for (auto effect : food->effects) {
				if (hunger->Survival_FoodRestoreHungerVerySmall == effect->baseEffect) {
					hunger->DecrementNeed(hunger->Survival_HungerRestoreVerySmallAmount->value);
				} else if (hunger->Survival_FoodRestoreHungerSmall == effect->baseEffect) {
					hunger->DecrementNeed(hunger->Survival_HungerRestoreSmallAmount->value);
				} else if (hunger->Survival_FoodRestoreHungerMedium == effect->baseEffect) {
					hunger->DecrementNeed(hunger->Survival_HungerRestoreMediumAmount->value);
				} else if (hunger->Survival_FoodRestoreHungerLarge == effect->baseEffect) {
					hunger->DecrementNeed(hunger->Survival_HungerRestoreLargeAmount->value);		
				}
			}
		}
	}

	class OnSleepStartEventHandler : public RE::BSTEventSink<RE::TESSleepStartEvent>
	{
	public:
		static OnSleepStartEventHandler* GetSingleton()
		{
			static OnSleepStartEventHandler singleton;
			return std::addressof(singleton);
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

	private:
		OnSleepStartEventHandler() = default;
	};

	class OnSleepStopEventHandler : public RE::BSTEventSink<RE::TESSleepStopEvent>
	{
	public:
		static OnSleepStopEventHandler* GetSingleton()
		{
			static OnSleepStopEventHandler singleton;
			return std::addressof(singleton);
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

	private:
		OnSleepStopEventHandler() = default;
	};

	class OnEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent>
	{
	public:
		static OnEquipEventHandler* GetSingleton()
		{
			static OnEquipEventHandler singleton;
			return std::addressof(singleton);
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event,[[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override
		{
			if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef()) {
				return RE::BSEventNotifyControl::kContinue;
			}

			auto alchemyItem = RE::TESForm::LookupByID<RE::AlchemyItem>(a_event->baseObject);

			if (alchemyItem && alchemyItem->IsFood()) {
				ProcessHungerOnEquipEvent(alchemyItem);
			}

			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnEquipEventHandler::GetSingleton());
		}

	private:
		OnEquipEventHandler() = default;
	};

	inline static void Register()
	{
		OnSleepStartEventHandler::Register();
		OnSleepStopEventHandler::Register();
		OnEquipEventHandler::Register();
	}
}
