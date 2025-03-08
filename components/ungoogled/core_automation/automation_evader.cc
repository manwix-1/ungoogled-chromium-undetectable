#include "components/ungoogled/core_automation/automation_evader.h"
#include "base/rand_util.h"
#include "v8/include/v8.h"

namespace ungoogled {

bool AutomationEvader::Initialize(const EvaderConfig& config) {
    config_ = config;
    rng_ = std::make_unique<base::RandomNumberGenerator>();
    return true;
}

void AutomationEvader::RemoveCDPTraces() {
    if (!config_.cdp.remove_protocol_handlers)
        return;

    // Remove DevTools protocol handlers
    content::DevToolsAgentHost::RemoveAllListeners();

    // Modify runtime flags to hide debugger
    if (config_.cdp.hide_debugger) {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        if (isolate) {
            v8::HandleScope scope(isolate);
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            v8::Local<v8::Object> global = context->Global();

            // Remove debugger properties
            global->Delete(context, 
                v8::String::NewFromUtf8(isolate, "debugger").ToLocalChecked());
            global->Delete(context,
                v8::String::NewFromUtf8(isolate, "chrome").ToLocalChecked());
        }
    }
}

void AutomationEvader::ModifyRuntimeEnvironment() {
    if (!config_.behavior.normalize_timing)
        return;

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if (!isolate)
        return;

    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> global = context->Global();

    // Modify performance timing
    if (config_.behavior.randomize_performance) {
        ModifyPerformanceAPI(isolate, global);
    }

    // Modify heap behavior
    if (config_.behavior.modify_heap) {
        v8::HeapStatistics heap_stats;
        isolate->GetHeapStatistics(&heap_stats);
        
        // Randomize heap limits
        size_t new_limit = heap_stats.heap_size_limit() +
                          base::RandInt(-1024 * 1024, 1024 * 1024);
        isolate->SetHeapLimitForDebugging(new_limit);
    }
}

void AutomationEvader::PatchNavigatorProperties() {
    if (!config_.properties.protect_navigator)
        return;

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if (!isolate)
        return;

    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> global = context->Global();

    // Modify navigator properties
    ModifyNavigator(isolate, global, {
        {"webdriver", false},
        {"selenium", false},
        {"driver", false},
        {"automationEnabled", false}
    });

    // Spoof plugins if configured
    if (config_.properties.spoof_plugins) {
        AddFakePlugins(isolate, global);
    }
}

void AutomationEvader::ModifyStackTraces() {
    if (!config_.behavior.vary_stack_traces)
        return;

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if (!isolate)
        return;

    // Modify stack trace formatting
    v8::Local<v8::StackTrace> stack_trace = v8::StackTrace::CurrentStackTrace(
        isolate, 10, v8::StackTrace::kDetailed);

    // Add noise to stack frames
    for (int i = 0; i < stack_trace->GetFrameCount(); i++) {
        v8::Local<v8::StackFrame> frame = stack_trace->GetFrame(isolate, i);
        if (frame->GetFunctionName()->Length() > 0) {
            // Add random noise to function names
            std::string name = *v8::String::Utf8Value(
                isolate, frame->GetFunctionName());
            name += "_" + base::RandBytesAsString(4);
            frame->SetFunctionName(v8::String::NewFromUtf8(
                isolate, name.c_str()).ToLocalChecked());
        }
    }
}

void AutomationEvader::EnhanceEvasion() {
  // Remove automation indicators
  RemoveCDPTraces();
  RemoveAutomationFlags();
  HideDebuggerPresence();
  NormalizePluginBehavior();
  
  // Add human-like characteristics
  SimulateUserInteraction();
  RandomizeTimingPatterns();
}

void AutomationEvader::RemoveAutomationFlags() {
  // Remove common automation flags
  RemoveNavigatorAutomation();
  RemoveWebDriverProperties();
  HideSeleniumTraces();
  
  // Normalize permissions
  StandardizePermissions();
  
  // Hide extension traces
  RemoveAutomationExtensions();
}

void AutomationEvader::SimulateUserInteraction() {
  // Add random mouse movements
  input_simulator_->EnableRandomMovements();
  
  // Randomize scroll behavior
  scroll_simulator_->EnableNaturalScrolling();
  
  // Add focus/blur events
  focus_simulator_->EnableRandomFocus();
}

}  // namespace ungoogled
