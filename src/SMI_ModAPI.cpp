#include "SMI_ModAPI.h"
#include <mutex>
#include "Utility.h"

namespace Messaging
{
    SmiInterface::~SmiInterface() noexcept {}

    void SmiInterface::SetInjuryHandlingEnabled(bool enabled) noexcept
    {
        logger::info("API Call: Setting injury pen handling {}", enabled);
        auto                        util = Utility::GetSingleton();
        util->handleInjuries             = enabled;
    }
}
