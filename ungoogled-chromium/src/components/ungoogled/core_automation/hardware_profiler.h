#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_HARDWARE_PROFILER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_HARDWARE_PROFILER_H_

#include <memory>
#include <string>
#include <vector>
#include "base/values.h"

namespace ungoogled {

struct HardwareProfile {
  // GPU related
  std::string gpu_vendor;
  std::string gpu_renderer;
  int32_t max_texture_size;
  std::vector<std::string> supported_extensions;
  
  // Audio related
  std::vector<std::string> audio_devices;
  float sample_rate;
  int32_t channel_count;
  
  // Screen related
  int32_t screen_width;
  int32_t screen_height;
  float device_pixel_ratio;
  
  // CPU related
  int32_t logical_processors;
  int32_t memory_gb;
  std::string platform;
};

class HardwareProfiler {
 public:
  HardwareProfiler();
  ~HardwareProfiler();

  // Prevent copying
  HardwareProfiler(const HardwareProfiler&) = delete;
  HardwareProfiler& operator=(const HardwareProfiler&) = delete;

  // Core profiling methods
  HardwareProfile GenerateProfile();
  void ApplyProfile(const HardwareProfile& profile);
  
  // Specific hardware masking methods
  base::Value GetMaskedWebGLInfo() const;
  base::Value GetMaskedAudioCapabilities() const;
  base::Value GetMaskedScreenMetrics() const;
  base::Value GetMaskedPlatformInfo() const;

  // Profile validation and normalization
  bool ValidateProfile(const HardwareProfile& profile) const;
  HardwareProfile NormalizeProfile(const HardwareProfile& profile) const;

  // Configuration
  void SetFingerprintingProtectionLevel(int level);
  bool IsProtectionEnabled() const;

 private:
  // Internal helper methods
  void InitializeDefaultProfile();
  void UpdateMaskingParameters();
  std::string GenerateConsistentDeviceId() const;
  
  // Hardware detection methods
  base::Value DetectRealGPUInfo() const;
  base::Value DetectRealAudioDevices() const;
  base::Value DetectRealScreenMetrics() const;
  base::Value DetectRealPlatformInfo() const;

  // Internal state
  HardwareProfile current_profile_;
  int protection_level_;
  bool protection_enabled_;
  std::string device_id_;
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_HARDWARE_PROFILER_H_