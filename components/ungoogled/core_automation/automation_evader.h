#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_AUTOMATION_EVADER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_AUTOMATION_EVADER_H_

#include "base/memory/weak_ptr.h"
#include "components/ungoogled/core_automation/evasion_config.h"

namespace ungoogled {

class AutomationEvader {
public:
    struct EvaderConfig {
        struct CDPConfig {
            bool remove_protocol_handlers = true;
            bool hide_debugger = true;
            bool mask_websocket = true;
            bool randomize_ids = true;
        } cdp;

        struct WebDriverConfig {
            bool remove_properties = true;
            bool mask_navigator = true;
            bool spoof_permissions = true;
        } webdriver;

        struct BehaviorConfig {
            bool randomize_timing = true;
            bool simulate_human = true;
            double jitter_ms = 50.0;
        } behavior;

        struct APIConfig {
            bool mask_automation_apis = true;
            bool protect_stack_traces = true;
            bool hide_debugging_props = true;
        } api;
    };

    bool Initialize(const EvaderConfig& config);
    void RemoveCDPTraces();
    void MaskWebDriver();
    void RandomizeBehavior();
    void ProtectAutomationAPIs();

private:
    void RemoveDebuggerProperties();
    void MaskNavigatorProperties();
    void RandomizeExecutionTiming();
    void HideStackTraces();
    void ProtectSensitiveAPIs();

    EvaderConfig config_;
    std::unique_ptr<base::RandomNumberGenerator> rng_;
    base::WeakPtrFactory<AutomationEvader> weak_factory_{this};
};

} // namespace ungoogled

#endif // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_AUTOMATION_EVADER_H_
