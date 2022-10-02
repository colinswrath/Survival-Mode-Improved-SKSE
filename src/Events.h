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
		Hours = RE::Calendar::GetSingleton()->GetHoursPassed() - Hours;

		logger::info("Hours slept "+std::to_string(Hours));

		auto hunger = NeedHunger::GetSingleton();
		auto exhaustion = NeedExhaustion::GetSingleton();

		if (!exhaustion->CurrentlyStopped) {
			NeedExhaustion::GetSingleton()->DecreaseExhaustion(Hours);
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

	inline static void Register()
	{
		OnSleepStartEventHandler::Register();
		OnSleepStopEventHandler::Register();
	}
}
