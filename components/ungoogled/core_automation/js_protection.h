#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_JS_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_JS_PROTECTION_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class JSProtection : public base::RefCountedThreadSafe<JSProtection> {
 public:
  struct APIConfig {
    bool override_performance;
    bool modify_timing_resolution;
    bool protect_battery_api;
    bool spoof_memory_info;
    bool intercept_webrtc;
  };

  struct WebAssemblyConfig {
    bool disable_wasm;
    bool limit_memory;
    bool intercept_imports;
    bool modify_capabilities;
  };

  bool ApplyJSProtection(const APIConfig& config);
  bool ConfigureWebAssembly(const WebAssemblyConfig& config);
  
 private:
  bool OverrideNativeAPIs();
  bool ModifyTimingBehavior();
  bool InterceptWebRTC();
  bool ConfigureWasmRuntime();
  
  std::unique_ptr<JSInterceptor> js_interceptor_;
  std::unique_ptr<WasmController> wasm_controller_;
};

}  // namespace ungoogled

#endif