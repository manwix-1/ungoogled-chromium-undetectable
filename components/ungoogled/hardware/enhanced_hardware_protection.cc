#include "components/ungoogled/hardware/enhanced_hardware_protection.h"
#include "base/rand_util.h"

namespace ungoogled {

void EnhancedHardwareProtection::Initialize(const ProtectionConfig& config) {
    config_ = config;
    hardware_profiler_ = std::make_unique<HardwareProfiler>();
}

void EnhancedHardwareProtection::ApplyProtection() {
    // Apply CPU protection
    if (config_.cpu.mask_vendor_string) {
        hardware_profiler_->SetCPUVendor("GenuineIntel");
        hardware_profiler_->SetCPUBrand("Intel(R) Core(TM) i7-1165G7 @ 2.80GHz");
    }

    if (config_.cpu.randomize_features) {
        hardware_profiler_->RandomizeCPUFeatures({
            .avx = true,
            .sse4_2 = true,
            .aes = true,
            .base_features = true
        });
    }

    // Apply GPU protection
    if (config_.gpu.mask_vendor) {
        hardware_profiler_->SetGPUVendor("Intel");
        hardware_profiler_->SetGPURenderer("Intel(R) Iris(R) Xe Graphics");
    }

    if (config_.gpu.normalize_performance) {
        hardware_profiler_->NormalizeGPUMetrics({
            .max_texture_size = 16384,
            .max_viewport_dims = {16384, 16384},
            .max_renderbuffer_size = 16384
        });
    }

    // Apply timing protection
    RandomizeTimingBehavior();
}

void EnhancedHardwareProtection::RandomizeTimingBehavior() {
    double base_variance = config_.cpu.rdtsc_variance;
    
    // Add noise to RDTSC readings
    hardware_profiler_->AddTimingNoise({
        .rdtsc_noise = base_variance,
        .branch_noise = config_.cpu.branch_variance,
        .cache_noise = base_variance * 0.5,
        .interrupt_noise = base_variance * 0.25
    });
}

bool EnhancedHardwareProtection::ValidateProtection() const {
    return hardware_profiler_->ValidateProfile();
}

}  // namespace ungoogled