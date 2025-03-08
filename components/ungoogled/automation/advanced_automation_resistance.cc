#include "components/ungoogled/automation/advanced_automation_resistance.h"
#include "base/rand_util.h"

namespace ungoogled {

void AdvancedAutomationResistance::Initialize(const ResistanceConfig& config) {
    config_ = config;
    behavior_simulator_ = std::make_unique<BehaviorSimulator>();
    automation_evader_ = std::make_unique<AutomationEvader>();
}

void AdvancedAutomationResistance::ApplyResistance() {
    if (config_.behavior.randomize_input_timing) {
        behavior_simulator_->ConfigureInputTiming({
            .min_delay = 10.0,  // ms
            .max_delay = config_.behavior.input_delay_range,
            .distribution = "normal"
        });
    }

    if (config_.detection.mask_automation_flags) {
        automation_evader_->MaskAutomationFlags({
            .remove_webdriver = true,
            .hide_chrome_automation = true,
            .mask_debugger = true,
            .normalize_navigator = true
        });
    }

    if (config_.detection.normalize_performance) {
        NormalizePerformanceMetrics();
    }

    if (config_.detection.randomize_heap) {
        RandomizeHeapBehavior();
    }
}

void AdvancedAutomationResistance::NormalizePerformanceMetrics() {
    automation_evader_->NormalizePerformance({
        .timing_resolution = 5.0,  // ms
        .memory_noise = 0.05,     // 5% variance
        .cpu_noise = 0.02        // 2% variance
    });
}

void AdvancedAutomationResistance::RandomizeHeapBehavior() {
    automation_evader_->ModifyHeapBehavior({
        .allocation_noise = 0.01,  // 1% allocation variance
        .gc_timing_noise = 0.05,   // 5% GC timing variance
        .memory_limit_noise = 0.02 // 2% memory limit variance
    });
}

bool AdvancedAutomationResistance::ValidateResistance() const {
    return automation_evader_->ValidateProtections() &&
           behavior_simulator_->ValidateConfiguration();
}

}  // namespace ungoogled