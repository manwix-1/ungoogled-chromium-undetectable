#include "components/ungoogled/features.h"
#include "base/feature_list.h"

namespace features {

// Define the unified protection feature
const base::Feature kUnifiedFingerprintingProtection{
    "UnifiedFingerprintingProtection",
    base::FEATURE_DISABLED_BY_DEFAULT
};

}  // namespace features