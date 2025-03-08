#include "components/ungoogled/fingerprinting_protection.h"
#include "components/ungoogled/features.h"
#include "base/command_line.h"
#include "components/ungoogled/ungoogled_switches.h"

namespace ungoogled {

bool IsProtectionEnabled(const std::string& protection_switch) {
    auto* cmd_line = base::CommandLine::ForCurrentProcess();
    
    // Check if unified protection is enabled (via flag or command line)
    if (base::FeatureList::IsEnabled(features::kUnifiedFingerprintingProtection) ||
        cmd_line->HasSwitch(switches::kUnifiedFingerprintingProtection)) {
        return true;
    }
    
    // Check individual protection (via flag or command line)
    return cmd_line->HasSwitch(protection_switch);
}

bool IsFontProtectionEnabled() {
    return IsProtectionEnabled(switches::kFingerprintingFontNoise);
}

bool IsAudioProtectionEnabled() {
    return IsProtectionEnabled(switches::kFingerprintingAudioNoise);
}

bool IsHardwareProtectionEnabled() {
    return IsProtectionEnabled(switches::kFingerprintingHardwareConcurrency);
}

bool IsMemoryProtectionEnabled() {
    return IsProtectionEnabled(switches::kFingerprintingMemoryNoise);
}

}  // namespace ungoogled