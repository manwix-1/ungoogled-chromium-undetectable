#ifndef COMPONENTS_UNGOOGLED_FINGERPRINTING_CONFIG_H_
#define COMPONENTS_UNGOOGLED_FINGERPRINTING_CONFIG_H_

#include "base/values.h"
#include <string>
#include <vector>

namespace ungoogled {

struct HardwareConfig {
  std::string cpu_cores = "4";
  std::string cpu_class = "unknown";
  std::string gpu_vendor = "Generic";
  std::string gpu_renderer = "Generic Renderer";
  float memory_gb = 8.0f;
  bool spoof_hardware_concurrency = true;
};

struct NetworkConfig {
  bool spoof_ip_addresses = true;
  bool randomize_local_ips = true;
  bool protect_network_interfaces = true;
  std::vector<std::string> allowed_ip_ranges;
};

struct BrowserConfig {
  bool spoof_timezone = true;
  bool spoof_language = false;
  std::string forced_language;
  bool protect_plugins = true;
  bool protect_mime_types = true;
  std::vector<std::string> allowed_plugins;
};

struct MediaConfig {
  bool protect_media_devices = true;
  bool spoof_media_capabilities = true;
  bool block_audio_fingerprinting = true;
  float audio_noise_level = 0.02f;
};

struct CanvasConfig {
  float coordinate_noise = 0.01f;
  float dimension_noise = 0.01f;
  float pixel_noise = 0.02f;
  bool protect_readback = true;
  bool noise_on_gradient = true;
};

struct WebGLConfig {
  bool randomize_extensions = true;
  bool spoof_capabilities = true;
  bool protect_parameters = true;
  float precision_reduction = 0.1f;
};

struct PerformanceConfig {
  bool protect_timing_api = true;
  bool protect_resource_timing = true;
  bool protect_navigation_timing = true;
  bool protect_frame_timing = true;
  float timing_noise_range = 0.05f;
  bool quantize_timestamps = true;
  uint32_t timestamp_resolution_ms = 100;
  bool normalize_performance_entries = true;
};

struct FontConfig {
  bool standardize_font_metrics = true;
  bool protect_font_enumeration = true;
  bool normalize_text_measurement = true;
  std::vector<std::string> allowed_fonts;
  bool spoof_system_fonts = true;
  float font_metrics_noise = 0.02f;
  bool protect_font_fingerprinting = true;
  bool standardize_font_rendering = true;
};

struct BatteryConfig {
  bool spoof_battery_status = true;
  bool randomize_charging_time = true;
  float charging_time = 3600.0f;
  float discharging_time = 14400.0f;
  float battery_level = 0.75f;
  bool is_charging = true;
};

struct PointerConfig {
  bool normalize_pointer_events = true;
  bool protect_pointer_features = true;
  float coordinate_noise = 0.02f;
  bool standardize_pressure = true;
  bool normalize_tilt = true;
  bool protect_touch_events = true;
};

struct CryptoConfig {
  bool protect_random_values = true;
  bool normalize_subtle_crypto = true;
  bool protect_random_uuid = true;
  bool standardize_crypto_capabilities = true;
};

struct WebRTCConfig {
  bool force_relay_only = true;
  bool disable_mdns = true;
  bool protect_ice_servers = true;
  bool hide_local_candidates = true;
  bool standardize_sdp = true;
  bool protect_peer_connection = true;
  std::vector<std::string> allowed_stun_servers;
  std::vector<std::string> allowed_turn_servers;
};

struct TimingConfig {
  bool reduce_precision = true;
  float time_noise_range = 0.1f;
  bool protect_performance_counters = true;
  bool protect_animation_frame = true;
};

struct FingerprintingConfig {
  HardwareConfig hardware;
  NetworkConfig network;
  BrowserConfig browser;
  MediaConfig media;
  CanvasConfig canvas;
  WebGLConfig webgl;
  FontConfig font;
  TimingConfig timing;
  PerformanceConfig performance;
  BatteryConfig battery;
  PointerConfig pointer;
  CryptoConfig crypto;
  WebRTCConfig webrtc;

  // Serialization
  base::Value::Dict ToDict() const;
  bool FromDict(const base::Value::Dict& dict);
};

} // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_FINGERPRINTING_CONFIG_H_
