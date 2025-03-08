#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_GRAPHICS_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_GRAPHICS_PROTECTION_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class GraphicsProtection : public base::RefCountedThreadSafe<GraphicsProtection> {
 public:
  struct WebGLConfig {
    bool randomize_parameters;
    bool spoof_extensions;
    bool modify_precision;
    std::string vendor_override;
    std::string renderer_override;
    bool simulate_hardware_acceleration;
  };

  struct CanvasConfig {
    bool add_noise;
    double noise_amplitude;
    bool randomize_text_metrics;
    bool modify_readback_data;
    bool consistent_noise;
  };

  bool ApplyWebGLProtection(const WebGLConfig& config);
  bool ApplyCanvasProtection(const CanvasConfig& config);
  
 private:
  bool ModifyWebGLContext();
  bool InterceptCanvasAPI();
  bool GenerateConsistentNoise();
  bool SimulateGPUCharacteristics();
  
  std::unique_ptr<WebGLInterceptor> webgl_interceptor_;
  std::unique_ptr<CanvasManager> canvas_manager_;
};

}  // namespace ungoogled

#endif