// Add to kFeatureEntries array
const FeatureEntry kFeatureEntries[] = {
    // Existing entries...
    
    {"unified-fingerprinting-protection",
     "Enable unified fingerprinting protection",
     "Enables all fingerprinting protection features including font, audio, hardware, and memory protections",
     kOsAll, FEATURE_VALUE_TYPE(features::kUnifiedFingerprintingProtection)},
    
    {"fingerprinting-font-noise",
     "Enable font fingerprinting protection",
     "Normalizes font metrics and availability to prevent fingerprinting",
     kOsAll, SINGLE_VALUE_TYPE(switches::kFingerprintingFontNoise)},
    
    {"fingerprinting-audio-noise",
     "Enable audio context fingerprinting protection",
     "Adds noise to AudioContext measurements to prevent fingerprinting",
     kOsAll, SINGLE_VALUE_TYPE(switches::kFingerprintingAudioNoise)},
    
    {"fingerprinting-hardware-concurrency",
     "Enable hardware concurrency normalization",
     "Reports a normalized value for navigator.hardwareConcurrency",
     kOsAll, SINGLE_VALUE_TYPE(switches::kFingerprintingHardwareConcurrency)},
    
    {"fingerprinting-memory-noise",
     "Enable memory detection prevention",
     "Adds noise to memory-related APIs to prevent fingerprinting",
     kOsAll, SINGLE_VALUE_TYPE(switches::kFingerprintingMemoryNoise)},
};
