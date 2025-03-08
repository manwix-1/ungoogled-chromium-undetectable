#ifndef COMPONENTS_UNGOOGLED_SWITCHES_H_
#define COMPONENTS_UNGOOGLED_SWITCHES_H_

namespace switches {

// Enables all fingerprinting protections
const char kUnifiedFingerprintingProtection[] = "unified-fingerprinting-protection";

// Individual protection switches
const char kFingerprintingFontNoise[] = "fingerprinting-font-noise";
const char kFingerprintingAudioNoise[] = "fingerprinting-audio-noise";
const char kFingerprintingHardwareConcurrency[] = "fingerprinting-hardware-concurrency";
const char kFingerprintingMemoryNoise[] = "fingerprinting-memory-noise";

}  // namespace switches

#endif  // COMPONENTS_UNGOOGLED_SWITCHES_H_
