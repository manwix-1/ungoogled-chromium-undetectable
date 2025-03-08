#ifndef COMPONENTS_UNGOOGLED_HARDWARE_ENHANCED_HARDWARE_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_HARDWARE_ENHANCED_HARDWARE_PROTECTION_H_

#include <memory>
#include "base/values.h"

namespace ungoogled {

class EnhancedHardwareProtection {
public:
    struct ProtectionConfig {
        struct CPUConfig {
            double rdtsc_variance;      // RDTSC timing variance
            bool mask_vendor_string;    // Hide real CPU vendor
            bool randomize_features;    // Randomize CPU features
            double branch_variance;     // Branch timing variance
        };

        struct GPUConfig {
            bool mask_vendor;           // Hide GPU vendor
            bool normalize_performance; // Standardize GPU metrics
            bool randomize_extensions;  // Randomize GPU extensions
            double precision_noise;     // WebGL precision noise
        };

        CPUConfig cpu;
        GPUConfig gpu;
    };

    void Initialize(const ProtectionConfig& config);
    void ApplyProtection();
    bool ValidateProtection() const;

private:
    void MaskCPUCharacteristics();
    void NormalizeGPUBehavior();
    void RandomizeTimingBehavior();
    void ObfuscateHardwareFeatures();

    ProtectionConfig config_;
    std::unique_ptr<HardwareProfiler> hardware_profiler_;
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_HARDWARE_ENHANCED_HARDWARE_PROTECTION_H_