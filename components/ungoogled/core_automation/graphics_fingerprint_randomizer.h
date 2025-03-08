#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_GRAPHICS_FINGERPRINT_RANDOMIZER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_GRAPHICS_FINGERPRINT_RANDOMIZER_H_

#include "base/memory/ref_counted.h"
#include "gpu/command_buffer/service/context_state.h"
#include "ui/gl/gl_context.h"

namespace ungoogled {

class GraphicsFingerprintRandomizer : public base::RefCountedThreadSafe<GraphicsFingerprintRandomizer> {
 public:
  struct WebGLConfig {
    // Core WebGL modifications
    struct {
      std::vector<std::string> vendor_rotations;    // Rotating vendor strings
      std::vector<std::string> renderer_rotations;  // Rotating renderer strings
      bool normalize_precision;     // Normalize shader precision
      bool standardize_extensions;  // Standardize extension list
      uint32_t max_texture_size;   // Normalized texture size
    } core;

    // Advanced parameter control
    struct {
      bool randomize_max_values;   // Randomize max parameters
      bool modify_capabilities;    // Modify capability flags
      bool spoof_performance;      // Spoof performance metrics
      bool normalize_buffers;      // Normalize buffer sizes
    } parameters;

    // Shader manipulation
    struct {
      bool inject_noise;           // Add shader noise
      bool modify_precision;       // Modify precision qualifiers
      bool standardize_versions;   // Standardize GLSL versions
      bool transform_code;         // Transform shader code
    } shaders;
  };

  bool Initialize(const WebGLConfig& config);
  
  // Core WebGL modifications
  void PatchWebGLContext(gl::GLContext* context);
  void ModifyShaderSource(std::string* source);
  void NormalizeParameters(gpu::ContextState* state);
  
  // Canvas protection
  void ApplyCanvasNoise(uint8_t* data, size_t length);
  void ModifyImageData(ImageData* data);
  void RandomizeReadback(v8::Local<v8::ArrayBuffer>* buffer);

private:
  WebGLConfig config_;
  std::unique_ptr<RandomNumberGenerator> rng_;
};

}  // namespace ungoogled

#endif