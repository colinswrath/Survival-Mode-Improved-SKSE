#pragma once

#include "Utility.h"

struct AvPenaltyHandler
{
    std::optional <std::function<float()>> calculatePenalty;
    RE::ActorValue         trackedAV;
    RE::ActorValue         affectedAV;
    RE::TESGlobal*         uiGlobal;
    NeedBase*              associatedNeed{ nullptr }; //For non starfrost AV Penalties
};

class AvPenaltyManager
{
public:
    RE::TESGlobal* StaminaUIGlobal;
    RE::TESGlobal* HealthUIGlobal;
    RE::TESGlobal* MagickaUIGlobal;

    static AvPenaltyManager* GetSingleton()
    {
        static AvPenaltyManager avPenManager;
        return &avPenManager;
    }

    void InitializeHandlers()
    {
        auto util = Utility::GetSingleton();

        //If old starfrost, hunger does not effect AVs
        // if new starfrost, exhaustion/hunger effect stamina and magicka

        bool hasBnB4 = util->BladeAndBlunt4;
        
        //If starfrost
        if (util->starfrostInstalled)
        {
            if (util->starfrostVer >= ModVersion(2,0,0))
            {
                staminaHandler = { [this] { return CalculateExhaustionPenPercent() + CalculateHungerPenPercent(); }, RE::ActorValue::kVariable02, RE::ActorValue::kStamina, StaminaUIGlobal, nullptr };
                healthHandler  = {
                        [this, hasBnB4] {
                            float pen = CalculateColdPenPercent();
                            if (hasBnB4)
                                pen += CalculateHealthPenaltyPercentBladeAndBlunt();
                            return pen;
                    },
                    RE::ActorValue::kVariable04, RE::ActorValue::kHealth, HealthUIGlobal, nullptr };
                magickaHandler = { [this] { return CalculateExhaustionPenPercent() + CalculateHungerPenPercent(); }, RE::ActorValue::kVariable03, RE::ActorValue::kMagicka, MagickaUIGlobal, nullptr };
            }
            else
            {
                staminaHandler = { [this] { return CalculateExhaustionPenPercent(); }, RE::ActorValue::kVariable02, RE::ActorValue::kStamina, StaminaUIGlobal, nullptr };
                healthHandler  = { hasBnB4 ?
                    std::optional{ std::function<float()>{ [this] { return CalculateHealthPenaltyPercentBladeAndBlunt();}}} : std::nullopt,
                                  RE::ActorValue::kVariable04, RE::ActorValue::kHealth, HealthUIGlobal, nullptr };
                magickaHandler = { [this] { return CalculateExhaustionPenPercent(); }, RE::ActorValue::kVariable03, RE::ActorValue::kMagicka, MagickaUIGlobal, nullptr };
            }
        }
        else
        {
            auto hunger    = NeedHunger::GetSingleton();
            staminaHandler = { [this, hunger] { return GetPenaltyPercentAmount(hunger); }, hunger->NeedPenaltyAV, hunger->ActorValPenaltyAttribute, hunger->NeedPenaltyUIGlobal, hunger };

            auto cold = NeedCold::GetSingleton();
            healthHandler = {
                [this, cold, hasBnB4] {
                    float pen = GetPenaltyPercentAmount(cold);
                    if (hasBnB4)
                        pen += CalculateHealthPenaltyPercentBladeAndBlunt();
                    return pen;
                },
                cold->NeedPenaltyAV, cold->ActorValPenaltyAttribute, cold->NeedPenaltyUIGlobal, cold };

            auto exhaustion = NeedExhaustion::GetSingleton();
            magickaHandler  = { [this, exhaustion] { return GetPenaltyPercentAmount(exhaustion); }, exhaustion->NeedPenaltyAV, exhaustion->ActorValPenaltyAttribute,
                               exhaustion->NeedPenaltyUIGlobal, exhaustion };
        }
    }

    void UpdateActorValuePenalties()
    {
        ApplyHandler(staminaHandler);
        ApplyHandler(healthHandler);
        ApplyHandler(magickaHandler);
    }

    void RemoveAllAvPenalties()
    {
        RemoveNeedAttributePenalty(NeedCold::GetSingleton());
        RemoveNeedAttributePenalty(NeedHunger::GetSingleton());
        RemoveNeedAttributePenalty(NeedExhaustion::GetSingleton());
    }

private:

#pragma region Starfrost


    float CalculateHungerPenPercent()
    {
        auto hunger = NeedHunger::GetSingleton();

        float hungerStage = hunger->CurrentNeedStage->value;
        auto  penPerc     = 0.0f;

        if (!hunger->CurrentlyStopped) {
            if (hungerStage == 3) {
                penPerc += 0.1f;
            }
            else if (hungerStage == 4) {
                penPerc += 0.25f;
            }
            else if (hungerStage >= 5) {
                penPerc += 0.50f;
            }
        }

        return penPerc;
    }

    float CalculateExhaustionPenPercent()
    {
        auto exhaustion = NeedExhaustion::GetSingleton();
        float exhaustionStage = exhaustion->CurrentNeedStage->value;
        auto  penPerc         = 0.0f;

        if (!exhaustion->CurrentlyStopped) {
            if (exhaustionStage == 3) {
                penPerc = 0.1f;
            }
            else if (exhaustionStage == 4) {
                penPerc = 0.25f;
            }
            else if (exhaustionStage >= 5) {
                penPerc = 0.50f;
            }
        }

        return penPerc;
    }

    float CalculateColdPenPercent()
    {
        auto cold = NeedCold::GetSingleton();

        auto penPerc = 0.0f;

        if (!cold->CurrentlyStopped) {
            float stage = cold->CurrentNeedStage->value;

            if (stage == 3) {
                penPerc = 0.1f;
            }
            else if (stage == 4) {
                penPerc = 0.25f;
            }
            else if (stage >= 5) {
                penPerc = 0.50f;
            }
        }

        return penPerc;
    }


#pragma endregion

#pragma region BladeAndBlunt

    float CalculateHealthPenaltyPercentBladeAndBlunt()
    {
        auto util    = Utility::GetSingleton();
        auto penPerc = 0.0f;

        if (util->BladeAndBlunt4 && util->BnBInjury1) {
            if ((util->MAG_InjuriesAndRest->value && util->MAG_InjuriesSMOnly->value == 0)
                || ((util->MAG_InjuriesSMOnly->value == 0) && (util->MAG_InjuriesAndRest->value > 0) && util->IsSurvivalEnabled()))
            {
                if (Utility::GetPlayer()->HasSpell(util->BnBInjury1)) {
                    penPerc += util->injury1AVPercent;
                }
                else if (Utility::GetPlayer()->HasSpell(util->BnBInjury2)) {
                    penPerc += util->injury2AVPercent;
                }
                else if (Utility::GetPlayer()->HasSpell(util->BnBInjury3)) {
                    penPerc += util->injury3AVPercent;
                }
            }
        }
        return penPerc;
    }

#pragma endregion

#pragma region Utility

    //Util

    void SetAttributePenaltyUIGlobal(float penaltyPerc, RE::TESGlobal* needUIGlobal)
    {
        auto newVal = penaltyPerc * 100.0f;
        newVal      = std::clamp(newVal, 0.0f, 100.0f);

        needUIGlobal->value = newVal;
    }

    float GetMaxAttributeAv(RE::ActorValue avPenAttribute, RE::ActorValue needPenaltyAv)
    {
        return (Utility::GetPlayer()->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, avPenAttribute)
                + Utility::GetPlayer()->AsActorValueOwner()->GetPermanentActorValue(avPenAttribute) + Utility::GetPlayer()->AsActorValueOwner()->GetActorValue(needPenaltyAv));
    }

#pragma endregion

private:
    AvPenaltyHandler  staminaHandler;
    AvPenaltyHandler  healthHandler;
    AvPenaltyHandler  magickaHandler;

    void ApplyHandler(const AvPenaltyHandler& handler)
    {
        if (handler.associatedNeed)
        {
            if (handler.associatedNeed->NeedAvPenDisabled->value == 1.0f || handler.associatedNeed->CurrentlyStopped)
            {
                RemoveNeedAttributePenalty(handler.associatedNeed);
                return;
            }
        }

        auto player   = Utility::GetPlayer();
        auto maxPenAv = GetMaxAttributeAv(handler.affectedAV, handler.trackedAV);

        float penPerc = 0.0f;
        if (handler.calculatePenalty) {
            penPerc = std::clamp((*handler.calculatePenalty)(), 0.0f, 1.0f);
        }
        else {
            return;
        }

        float lastPenaltyMag = player->AsActorValueOwner()->GetActorValue(handler.trackedAV);
        float newPenaltyMag  = std::roundf(maxPenAv * penPerc);
        newPenaltyMag        = std::min(newPenaltyMag, maxPenAv);

        float magDelta = lastPenaltyMag - newPenaltyMag;

        player->AsActorValueOwner()->SetActorValue(handler.trackedAV, newPenaltyMag);
        player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, handler.affectedAV, magDelta);

        SetAttributePenaltyUIGlobal(penPerc, handler.uiGlobal);
    }

    virtual void RemoveNeedAttributePenalty(NeedBase* need)
    {
        float currentPenaltyMag = Utility::GetPlayer()->AsActorValueOwner()->GetActorValue(need->NeedPenaltyAV);

        if (currentPenaltyMag > 0) {
            Utility::GetPlayer()->AsActorValueOwner()->SetActorValue(need->NeedPenaltyAV, 0.0f);
            SetAttributePenaltyUIGlobal(0.0f, need->NeedPenaltyUIGlobal);
            Utility::GetPlayer()->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, need->ActorValPenaltyAttribute, currentPenaltyMag);
        }
    }

    virtual float GetPenaltyPercentAmount(NeedBase* need)
    {
        auto util    = Utility::GetSingleton();
        auto penalty = (need->CurrentNeedValue->value - need->NeedStage2->value - 1) / (need->NeedMaxValue->value - need->NeedStage2->value - 1);
        penalty      = std::clamp(penalty, 0.0f, util->MaxAvPenaltyPercent);

        return penalty;
    }
};
