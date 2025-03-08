#include "components/ungoogled/core_automation/hardware_profiler.h"
#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"
#include "base/system/sys_info.h"
#include "components/ungoogled/features.h"
#include "crypto/random.h"

namespace ungoogled {

namespace {

constexpr char kDefaultGPUVendor[] = "Intel Inc.";
constexpr char kDefaultGPURenderer[] = "Intel Iris Graphics";
constexpr int kDefaultMaxTextureSize = 16384;
constexpr char kDefaultAudioDevice[] = "Default Audio Device";
constexpr float kDefaultSampleRate = 44100.0f;
constexpr int kDefaultChannelCount = 2;

// Common WebGL extensions to mask
const char* kCommonWebGLExtensions[] = {
    "ANGLE_instanced_arrays",
    "EXT_blend_minmax",
    "EXT_texture_filter_anisotropic",
    "WEBKIT_EXT_texture_filter_anisotropic",
    "OES_standard_derivatives",
    "OES_texture_float",
    "WEBGL_debug_renderer_info"
};

}  // namespace

HardwareProfiler::HardwareProfiler() 
    : protection_level_(0),
      protection_enabled_(false) {
  InitializeDefaultProfile();
  device_id_ = GenerateConsistentDeviceId();
}

HardwareProfiler::~HardwareProfiler() = default;

void HardwareProfiler::InitializeDefaultProfile() {
  current_profile_.gpu_vendor = kDefaultGPUVendor;
  current_profile_.gpu_renderer = kDefaultGPURenderer;
  current_profile_.max_texture_size = kDefaultMaxTextureSize;
  
  // Add common WebGL extensions
  for (const char* ext : kCommonWebGLExtensions) {
    current_profile_.supported_extensions.push_back(ext);
  }
  
  current_profile_.audio_devices.push_back(kDefaultAudioDevice);
  current_profile_.sample_rate = kDefaultSampleRate;
  current_profile_.channel_count = kDefaultChannelCount;
  
  // Use common desktop resolution
  current_profile_.screen_width = 1920;
  current_profile_.screen_height = 1080;
  current_profile_.device_pixel_ratio = 1.0f;
  
  // System info
  current_profile_.logical_processors = base::SysInfo::NumberOfProcessors();
  current_profile_.memory_gb = base::SysInfo::AmountOfPhysicalMemoryMB() / 1024;
  current_profile_.platform = base::SysInfo::OperatingSystemName();
}

HardwareProfile HardwareProfiler::GenerateProfile() {
  if (!protection_enabled_) {
    return current_profile_;
  }

  HardwareProfile profile = current_profile_;
  
  // Add controlled randomization based on device_id_
  uint32_t seed;
  crypto::RandBytes(&seed, sizeof(seed));
  
  // Adjust values within reasonable ranges
  profile.max_texture_size = kDefaultMaxTextureSize - (seed % 2048);
  profile.sample_rate = kDefaultSampleRate + (seed % 100) - 50;
  profile.device_pixel_ratio += (seed % 10) * 0.1f - 0.5f;
  
  return NormalizeProfile(profile);
}

void HardwareProfiler::ApplyProfile(const HardwareProfile& profile) {
  if (!ValidateProfile(profile)) {
    return;
  }
  current_profile_ = NormalizeProfile(profile);
}

base::Value HardwareProfiler::GetMaskedWebGLInfo() const {
  base::Value dict(base::Value::Type::DICTIONARY);
  
  if (protection_enabled_) {
    dict.SetStringKey("vendor", current_profile_.gpu_vendor);
    dict.SetStringKey("renderer", current_profile_.gpu_renderer);
    
    base::Value extensions(base::Value::Type::LIST);
    for (const auto& ext : current_profile_.supported_extensions) {
      extensions.Append(ext);
    }
    dict.SetKey("extensions", std::move(extensions));
  } else {
    auto real_info = DetectRealGPUInfo();
    dict = std::move(real_info);
  }
  
  return dict;
}

base::Value HardwareProfiler::GetMaskedAudioCapabilities() const {
  base::Value dict(base::Value::Type::DICTIONARY);
  
  if (protection_enabled_) {
    base::Value devices(base::Value::Type::LIST);
    for (const auto& device : current_profile_.audio_devices) {
      devices.Append(device);
    }
    dict.SetKey("devices", std::move(devices));
    dict.SetDoubleKey("sampleRate", current_profile_.sample_rate);
    dict.SetIntKey("channelCount", current_profile_.channel_count);
  } else {
    auto real_info = DetectRealAudioDevices();
    dict = std::move(real_info);
  }
  
  return dict;
}

void HardwareProfiler::SetFingerprintingProtectionLevel(int level) {
  protection_level_ = std::clamp(level, 0, 3);
  protection_enabled_ = (protection_level_ > 0);
  UpdateMaskingParameters();
}

std::string HardwareProfiler::GenerateConsistentDeviceId() const {
  // Generate a consistent but pseudo-random device ID
  std::string machine_id = base::SysInfo::HardwareModelName() +
                          base::SysInfo::OperatingSystemVersion();
  
  uint32_t hash = 0;
  for (char c : machine_id) {
    hash = hash * 31 + c;
  }
  
  return base::NumberToString(hash);
}

bool HardwareProfiler::ValidateProfile(const HardwareProfile& profile) const {
  // Validate reasonable ranges for hardware parameters
  if (profile.max_texture_size < 2048 || profile.max_texture_size > 32768) {
    return false;
  }
  
  if (profile.sample_rate < 44000 || profile.sample_rate > 48000) {
    return false;
  }
  
  if (profile.screen_width < 800 || profile.screen_width > 7680) {
    return false;
  }
  
  if (profile.screen_height < 600 || profile.screen_height > 4320) {
    return false;
  }
  
  if (profile.device_pixel_ratio < 0.5 || profile.device_pixel_ratio > 4.0) {
    return false;
  }
  
  return true;
}

HardwareProfile HardwareProfiler::NormalizeProfile(
    const HardwareProfile& profile) const {
  HardwareProfile normalized = profile;
  
  // Normalize values to reasonable ranges
  normalized.max_texture_size = std::clamp(profile.max_texture_size, 2048, 32768);
  normalized.sample_rate = std::clamp(profile.sample_rate, 44000.0f, 48000.0f);
  normalized.device_pixel_ratio = std::clamp(profile.device_pixel_ratio, 0.5f, 4.0f);
  
  return normalized;
}

}  // namespace ungoogled