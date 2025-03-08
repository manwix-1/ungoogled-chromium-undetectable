#include "components/ungoogled/fingerprinting/config.h"

namespace ungoogled {

FingerprintingConfig FingerprintingConfig::CreateDefault() {
  FingerprintingConfig config;
  
  // Set default allowed fonts
  config.font.allowed_fonts = {
    "Arial",
    "Times New Roman",
    "Courier New",
    "Georgia",
    "Verdana"
  };

  return config;
}

base::Value::Dict FingerprintingConfig::ToDict() const {
  base::Value::Dict dict;
  
  // Font config
  auto font_dict = base::Value::Dict();
  font_dict.Set("metrics_noise_range", font.metrics_noise_range);
  font_dict.Set("normalize_font_list", font.normalize_font_list);
  font_dict.Set("spoof_system_fonts", font.spoof_system_fonts);
  dict.Set("font", std::move(font_dict));

  // Audio config
  auto audio_dict = base::Value::Dict();
  audio_dict.Set("timing_noise_range", audio.timing_noise_range);
  audio_dict.Set("modify_frequency_data", audio.modify_frequency_data);
  audio_dict.Set("frequency_noise_range", audio.frequency_noise_range);
  audio_dict.Set("normalize_sample_rate", audio.normalize_sample_rate);
  audio_dict.Set("normalized_sample_rate", audio.normalized_sample_rate);
  dict.Set("audio", std::move(audio_dict));

  // Hardware config
  auto hardware_dict = base::Value::Dict();
  hardware_dict.Set("reported_cpu_cores", hardware.reported_cpu_cores);
  hardware_dict.Set("reported_memory_size", hardware.reported_memory_size);
  hardware_dict.Set("spoof_gpu_info", hardware.spoof_gpu_info);
  hardware_dict.Set("gpu_vendor", hardware.gpu_vendor);
  hardware_dict.Set("gpu_renderer", hardware.gpu_renderer);
  dict.Set("hardware", std::move(hardware_dict));

  // Memory config
  auto memory_dict = base::Value::Dict();
  memory_dict.Set("randomize_memory_values", memory.randomize_memory_values);
  memory_dict.Set("memory_noise_range", memory.memory_noise_range);
  memory_dict.Set("spoof_total_memory", memory.spoof_total_memory);
  memory_dict.Set("normalize_performance_values", memory.normalize_performance_values);
  dict.Set("memory", std::move(memory_dict));

  return dict;
}

bool FingerprintingConfig::FromDict(const base::Value::Dict& dict) {
  // Implementation of dictionary parsing
  return true;
}

} // namespace ungoogled