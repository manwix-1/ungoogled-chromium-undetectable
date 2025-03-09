#ifndef COMPONENTS_UNGOOGLED_HARDWARE_ENHANCED_HARDWARE_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_HARDWARE_ENHANCED_HARDWARE_PROTECTION_H_

#include <memory>
#include "base/values.h"

namespace ungoogled {

class EnhancedHardwareProtection {
public:
    struct HardwareConfig {
        struct CPUConfig {
            bool mask_instruction_timing = true;
            bool randomize_cache_access = true;
            bool protect_branch_prediction = true;
            double timing_noise_percentage = 0.03;
        };

        struct GPUConfig {
            bool normalize_capabilities = true;
            bool randomize_performance = true;
            bool mask_vendor_info = true;
            uint32_t performance_variance = 5;
        };

        struct SensorConfig {
            bool mask_precision = true;
            bool add_sensor_noise = true;
            double noise_amplitude = 0.02;
            uint32_t sampling_variance_ms = 2;
        };
    };

    void ApplyProtection(const HardwareConfig& config) {
        EnhanceCPUProtection(config.cpu);
        EnhanceGPUProtection(config.gpu);
        EnhanceSensorProtection(config.sensor);
    }

private:
    void EnhanceCPUProtection(const HardwareConfig::CPUConfig& config) {
        cpu_protector_.Configure({
            .rdtsc_noise = {
                .enabled = true,
                .variance = 0.02,
                .pattern = "gaussian",
                .granularity_ns = 100
            },
            .cache_protection = {
                .randomize_access = true,
                .add_dummy_loads = true,
                .pattern_complexity = 0.8
            },
            .branch_protection = {
                .randomize_prediction = true,
                .add_dummy_branches = true,
                .mask_history = true
            }
        });
    }
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_HARDWARE_ENHANCED_HARDWARE_PROTECTION_H_
