#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_BRAVE_FINGERPRINT_PROTECTIONS_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_BRAVE_FINGERPRINT_PROTECTIONS_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class BraveFingerprintProtections : public base::RefCountedThreadSafe<BraveFingerprintProtections> {
 public:
  struct ProtectionConfig {
    // Canvas protection
    bool farbling_level;  // 0=Off, 1=Low, 2=Medium, 3=High
    bool persistent_farbling;
    
    // WebGL protection
    bool webgl_noise;
    bool mask_webgl_vendor;
    bool randomize_webgl_params;
    
    // Audio protection
    bool audio_farbling;
    bool mask_audio_context;
    
    // Timezone protection
    bool spoof_timezone;
    bool use_system_timezone;
  };

  bool ApplyProtections(const ProtectionConfig& config);
  bool ConfigureFarbling(uint8_t farbling_level);
  
 private:
  bool ApplyCanvasFarbling();
  bool ApplyWebGLProtection();
  bool ApplyAudioFarbling();
  bool ApplyTimezoneProtection();
  
  std::unique_ptr<FarblingManager> farbling_manager_;
  std::unique_ptr<WebGLProtection> webgl_protection_;
  std::unique_ptr<AudioProtection> audio_protection_;
};

}  // namespace ungoogled

#endif