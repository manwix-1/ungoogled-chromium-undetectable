#ifndef COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_H_

namespace ungoogled {

bool IsFontProtectionEnabled();
bool IsAudioProtectionEnabled();
bool IsHardwareProtectionEnabled();
bool IsMemoryProtectionEnabled();

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_H_