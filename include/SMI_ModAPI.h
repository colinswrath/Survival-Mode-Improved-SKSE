#pragma once
#include "SMI_API.h"

namespace Messaging
{
    using SMIInterface1 = ::SMI_API::IVSmi1;

    class SmiInterface final : public SMIInterface1
    {
    private:
        virtual ~SmiInterface() noexcept;

    public:
        static SmiInterface* GetSingleton() noexcept
        {
            static SmiInterface singleton;
            return std::addressof(singleton);
        }

        virtual void SetInjuryHandlingEnabled(bool enabled) noexcept override;
    };
}
