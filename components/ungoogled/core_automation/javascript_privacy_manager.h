#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_JAVASCRIPT_PRIVACY_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_JAVASCRIPT_PRIVACY_MANAGER_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class JavaScriptPrivacyManager : public base::RefCountedThreadSafe<JavaScriptPrivacyManager> {
 public:
  struct JSConfig {
    // API Restrictions
    bool block_battery_api;
    bool block_bluetooth_api;
    bool block_gamepad_api;
    bool block_payment_api;
    bool block_serial_api;
    bool block_usb_api;
    
    // Timing Protection
    bool fuzzy_timing_api;
    bool spoof_performance_api;
    bool randomize_animation_timing;
    
    // Sensor Access
    bool block_ambient_light_sensor;
    bool block_proximity_sensor;
    bool block_accelerometer;
    bool block_gyroscope;
    
    // DOM Protection
    bool restrict_dom_rect;
    bool spoof_element_size;
    bool protect_clipboard_access;
  };

  bool Initialize(const JSConfig& config);
  bool ApplyJavaScriptProtections();
  bool ValidateProtections();

 private:
  bool RestrictAPIs();
  bool ProtectTiming();
  bool BlockSensors();
  bool SecureDOM();

  JSConfig config_;
  std::unique_ptr<APIBlocker> api_blocker_;
  std::unique_ptr<TimingProtector> timing_protector_;
  std::unique_ptr<SensorBlocker> sensor_blocker_;
  std::unique_ptr<DOMProtector> dom_protector_;
};

}  // namespace ungoogled

#endif