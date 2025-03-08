#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_FINGERPRINT_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_FINGERPRINT_MANAGER_H_

#include "base/memory/ref_counted.h"
#include "components/ungoogled/core_automation/network_fingerprint.h"

namespace ungoogled {

class FingerprintManager : public base::RefCountedThreadSafe<FingerprintManager> {
 public:
  struct CanvasConfig {
    bool noise_enabled;
    double noise_level;
    bool fake_dimensions;
    bool spoof_readback;
    bool modify_colors;
    bool randomize_transform;
    std::vector<uint8_t> noise_pattern;
  };

  struct WebGLConfig {
    bool spoof_vendor;
    bool mask_capabilities;
    bool randomize_precision;
    bool modify_parameters;
    std::string custom_vendor;
    std::vector<std::string> blocked_extensions;
  };

  struct NetworkConfig {
    NetworkFingerprint::TLSConfig tls;
    NetworkFingerprint::TCPConfig tcp;
    NetworkFingerprint::ProxyConfig proxy;
    bool randomize_headers;
    bool spoof_ip_address;
    bool rotate_user_agent;
  };

  bool Initialize(const CanvasConfig& canvas_config,
                 const WebGLConfig& webgl_config,
                 const NetworkConfig& network_config);

  // Canvas protection methods
  bool ApplyCanvasNoise(uint8_t* data, size_t length);
  bool ModifyCanvasReadback(v8::Local<v8::ArrayBuffer>* buffer);
  bool SpoofCanvasDimensions(int* width, int* height);

  // WebGL protection methods
  bool PatchWebGLContext(content::RenderFrameHost* frame);
  bool ModifyShaderPrecision(std::string* shader_source);
  bool SpoofWebGLParameters();

  // Network protection methods
  bool ConfigureNetworkFingerprint();
  bool SetupProxyChain();
  bool InitializeShadowTLS();

 private:
  // Canvas helpers
  void GenerateNoisePattern();
  void ApplyColorModification(uint8_t* pixel);
  void RandomizeTransformMatrix();

  // WebGL helpers
  void SetupVendorSpoofing();
  void ConfigureWebGLCapabilities();
  void ModifyPrecisionFormat();

  // Network helpers
  void SetupTLSCamouflage();
  void ConfigureProxyRotation();
  void InitializeNetworkObfuscation();

  CanvasConfig canvas_config_;
  WebGLConfig webgl_config_;
  NetworkConfig network_config_;
  
  std::unique_ptr<CanvasNoiseGenerator> noise_generator_;
  std::unique_ptr<WebGLSpoofer> webgl_spoofer_;
  std::unique_ptr<NetworkObfuscator> network_obfuscator_;
};

}  // namespace ungoogled

#endif
