#pragma once

#include "Utility.h"

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

    //Update all 3 needs
    //Take starfrost into account
        //TODO-Starfrost needs to check needs enabled
    //TODO-Hande individual needs being off
    //TODO-Handle SM being off


    void UpdateActorValuePenalties()
    {
        UpdateStaminaAvPenalty();
        UpdateHealthAvPenalty();
        UpdateMagickaAvPenalty();
    }

    void RemoveAllAvPenalties()
    {
        RemoveNeedAttributePenalty(NeedCold::GetSingleton());
        RemoveNeedAttributePenalty(NeedHunger::GetSingleton());
        RemoveNeedAttributePenalty(NeedExhaustion::GetSingleton());
    }
    
    void UpdateStaminaAvPenalty()
    {
        auto util = Utility::GetSingleton();

        // If starfrost
        if (util->starfrostInstalled) {
            ApplyStarfrostStaminaPenalty();
        }
        //If normal SMI
        else {
            ApplyNeedAttributePenalty(NeedHunger::GetSingleton());     
        }
    }


    void UpdateHealthAvPenalty()
    {
        auto util = Utility::GetSingleton();
        logger::info("Starfrost installed {0}", util->starfrostInstalled);
        // If starfrost
        if (util->starfrostInstalled) {
            ApplyStarfrostHealthPenalty();
        }
        else {
            // Cold Pen
            ApplyNeedAttributePenalty(NeedCold::GetSingleton());
        }
    }

    void UpdateMagickaAvPenalty()
    {
        auto util = Utility::GetSingleton();

        // If starfrost
        if (util->starfrostInstalled) {
            ApplyStarfrostMagickaPenalty();
        }
        else {
            // Exhaustion Pen
            ApplyNeedAttributePenalty(NeedExhaustion::GetSingleton());
        }
    }

private:

#pragma region Starfrost Stamina 

    void ApplyStarfrostStaminaPenalty()
    {
        auto staminaPenAv = RE::ActorValue::kVariable02;

        // Max
        auto maxPenAv = GetMaxAttributeAv(RE::ActorValue::kStamina, staminaPenAv);

        auto penPerc = CalculateStaminaPenaltyPercentStarfrost();

        // Magnitude
        float lastPenaltyMag = Utility::GetPlayer()->AsActorValueOwner()->GetActorValue(staminaPenAv);
        float newPenaltyMag  = std::roundf(maxPenAv * penPerc);

        if (newPenaltyMag > maxPenAv) {
            newPenaltyMag = maxPenAv;
        }
        auto magDelta = lastPenaltyMag - newPenaltyMag;

        // Set tracker av not actual damage
        Utility::GetPlayer()->AsActorValueOwner()->SetActorValue(staminaPenAv, newPenaltyMag);

        // Damage or restore AV
        Utility::GetPlayer()->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStamina, magDelta);

        SetAttributePenaltyUIGlobal(penPerc, StaminaUIGlobal);
    }

    float CalculateStaminaPenaltyPercentStarfrost()
    {
        auto util = Utility::GetSingleton();
        auto exhaustion = NeedExhaustion::GetSingleton();

        // Exhaustion
        auto  penPerc = 0.0f;
        float stage   = exhaustion->CurrentNeedStage->value;

        if (stage == 3) {
            penPerc = 0.1f;
        }
        else if (stage == 4) {
            penPerc = 0.25f;
        }
        else if (stage >= 5) {
            penPerc = 0.50f;
        }

        // Check starfrost hunger spells
        // Note: Hunger in starfrost is not managed internally by the DLL, instead its spell based in the esp
        if (Utility::GetPlayer()->HasSpell(util->StarfrostHunger1)) {
            penPerc += 0.10;
        }
        else if (Utility::GetPlayer()->HasSpell(util->StarfrostHunger2)) {
            penPerc += 0.25;
        }
        else if (Utility::GetPlayer()->HasSpell(util->StarfrostHunger3)) {
            penPerc += 0.50;
        }

        return penPerc;
    }

#pragma endregion

#pragma region Starfrost Health

    void ApplyStarfrostHealthPenalty()
    {
        auto healthPenAv = RE::ActorValue::kVariable04;

        // Max
        auto maxPenAv = GetMaxAttributeAv(RE::ActorValue::kHealth, healthPenAv);
        auto penPerc = CalculateHealthPenaltyPercentStarfrost();

        // Magnitude
        float lastPenaltyMag = Utility::GetPlayer()->AsActorValueOwner()->GetActorValue(healthPenAv);
        float newPenaltyMag  = std::roundf(maxPenAv * penPerc);

        if (newPenaltyMag > maxPenAv) {
            newPenaltyMag = maxPenAv;
        }
        auto magDelta = lastPenaltyMag - newPenaltyMag;

        // Set tracker av not actual damage
        Utility::GetPlayer()->AsActorValueOwner()->SetActorValue(healthPenAv, newPenaltyMag);

        // Damage or restore AV
        Utility::GetPlayer()->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, magDelta);

        SetAttributePenaltyUIGlobal(penPerc, HealthUIGlobal);
    }

    float CalculateHealthPenaltyPercentStarfrost()
    {
        auto util       = Utility::GetSingleton();
        auto cold = NeedCold::GetSingleton();
        auto penPerc = 0.0f;

        //Cold
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

        //BnB Installed
        if (util->BnBInjury1)
        {

            if ((util->MAG_InjuriesAndRest->value && util->MAG_InjuriesSMOnly->value == 0) || ((util->MAG_InjuriesSMOnly->value == 0) && (util->MAG_InjuriesAndRest->value > 0) && util->IsSurvivalEnabled()))
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

#pragma region Starfrost Magicka

    void ApplyStarfrostMagickaPenalty()
    {
        auto magickaPenAv = RE::ActorValue::kVariable03;

        // Max
        auto maxPenAv = GetMaxAttributeAv(RE::ActorValue::kMagicka, magickaPenAv);
        
        auto penPerc = CalculateMagickaPenaltyPercentStarfrost();

        // Magnitude
        float lastPenaltyMag = Utility::GetPlayer()->AsActorValueOwner()->GetActorValue(magickaPenAv);
        float newPenaltyMag  = std::roundf(maxPenAv * penPerc);

        if (newPenaltyMag > maxPenAv) {
            newPenaltyMag = maxPenAv;
        }
        auto magDelta = lastPenaltyMag - newPenaltyMag;

        // Set tracker av not actual damage
        Utility::GetPlayer()->AsActorValueOwner()->SetActorValue(magickaPenAv, newPenaltyMag);

        // Damage or restore AV
        Utility::GetPlayer()->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagicka, magDelta);

        SetAttributePenaltyUIGlobal(penPerc, MagickaUIGlobal);
    }

    float CalculateMagickaPenaltyPercentStarfrost()
    {
        auto util       = Utility::GetSingleton();
        auto exhaustion = NeedExhaustion::GetSingleton();

        // Exhaustion
        float stage   = exhaustion->CurrentNeedStage->value;
        auto  penPerc = 0.0f;

        if (stage == 3) {
            penPerc = 0.1f;
        }
        else if (stage == 4) {
            penPerc = 0.25f;
        }
        else if (stage >= 5) {
            penPerc = 0.50f;
        }

        // Check starfrost hunger spells
        // Note: Hunger in starfrost is not managed internally by the DLL, instead its spell based in the esp
        if (Utility::GetPlayer()->HasSpell(util->StarfrostHunger1)) {
            penPerc += 0.10;
        }
        else if (Utility::GetPlayer()->HasSpell(util->StarfrostHunger2)) {
            penPerc += 0.25;
        }
        else if (Utility::GetPlayer()->HasSpell(util->StarfrostHunger3)) {
            penPerc += 0.50;
        }

        return penPerc;
    }

#pragma endregion

#pragma region SMI Attribute Penalties


    virtual void ApplyNeedAttributePenalty(NeedBase* need)
    {
        if (need->NeedAvPenDisabled->value != 1.0f && !need->CurrentlyStopped) {
            float maxPenAv = GetMaxAttributeAv(need->ActorValPenaltyAttribute, need->NeedPenaltyAV);

            float penaltyPerc = GetPenaltyPercentAmount(need);

            float lastPenaltyMag = Utility::GetPlayer()->AsActorValueOwner()->GetActorValue(need->NeedPenaltyAV);
            float newPenaltyMag  = std::roundf(maxPenAv * penaltyPerc);

            if (newPenaltyMag > maxPenAv) {
                newPenaltyMag = maxPenAv;
            }
            auto magDelta = lastPenaltyMag - newPenaltyMag;

            // Set tracker av not actual damage
            Utility::GetPlayer()->AsActorValueOwner()->SetActorValue(need->NeedPenaltyAV, newPenaltyMag);

            // Damage or restore AV
            Utility::GetPlayer()->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, need->ActorValPenaltyAttribute, magDelta);

            SetAttributePenaltyUIGlobal(penaltyPerc, StaminaUIGlobal);
        }
        else {
            RemoveNeedAttributePenalty(need);
        }
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
};
