#include "components/ungoogled/core_automation/js_protection.h"
#include "v8/include/v8.h"
#include "base/rand_util.h"

namespace ungoogled {

bool JSProtection::ApplyJSProtection(const APIConfig& config) {
    if (!OverrideNativeAPIs(config))
        return false;
        
    if (!ModifyTimingBehavior(config))
        return false;
        
    if (config.protect_battery_api)
        ProtectBatteryAPI();
        
    if (config.spoof_memory_info)
        SpoofMemoryInfo();
        
    if (config.intercept_webrtc)
        InterceptWebRTC();
        
    return true;
}

bool JSProtection::ConfigureWebAssembly(const WebAssemblyConfig& config) {
    if (config.disable_wasm) {
        DisableWebAssembly();
    }
    
    if (config.limit_memory) {
        LimitWasmMemory({
            .max_pages = 256,
            .initial_pages = 1,
            .growth_rate = 2
        });
    }
    
    if (config.intercept_imports) {
        InterceptWasmImports({
            .block_unsafe = true,
            .sanitize_memory = true,
            .validate_types = true
        });
    }
    
    if (config.modify_capabilities) {
        ModifyWasmCapabilities({
            .simd = false,
            .threads = false,
            .exceptions = false
        });
    }
    
    return true;
}

bool JSProtection::OverrideNativeAPIs() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> global = context->Global();

    // Override performance.now()
    ModifyTimingAPI(isolate, global, "performance", {
        .granularity = 100,  // 100ms granularity
        .noise_range = 5,    // ±5ms random noise
        .monotonic = true
    });

    // Override Date.now()
    ModifyDateAPI(isolate, global, {
        .precision = 1000,   // 1s precision
        .offset_range = 50,  // ±50ms random offset
        .consistent = true
    });

    return true;
}

void JSProtection::ModifyTimingBehavior() {
    timing_modifier_.Configure({
        .high_res_time = {
            .enabled = false,
            .precision = 100,    // 100ms
            .noise = 0.05        // 5% noise
        },
        .animation_frame = {
            .jitter = true,
            .range_ms = {14, 18} // 60fps ±2ms
        },
        .intervals = {
            .minimum_delay = 4,  // 4ms minimum
            .randomization = 0.1 // 10% random delay
        }
    });
}

} // namespace ungoogled