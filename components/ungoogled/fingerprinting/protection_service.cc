#include "components/ungoogled/fingerprinting/protection_service.h"
#include <ctime>

namespace ungoogled {

FingerprintingProtectionService* FingerprintingProtectionService::GetInstance() {
  static base::NoDestructor<FingerprintingProtectionService> instance;
  return instance.get();
}

FingerprintingProtectionService::FingerprintingProtectionService() 
    : config_(FingerprintingConfig::CreateDefault()) {
  InitializeRNG();
}

FingerprintingProtectionService::~FingerprintingProtectionService() = default;

void FingerprintingProtectionService::InitializeRNG() {
  // Use a combination of factors for seeding
  std::random_device rd;
  std::seed_seq seeds{rd(), static_cast<unsigned>(std::time(nullptr))};
  rng_.seed(seeds);
}

double FingerprintingProtectionService::GenerateNoise(double range) const {
  std::uniform_real_distribution<double> dist(-range, range);
  return dist(rng_);
}

void FingerprintingProtectionService::ModifyFontMetrics(
    double* ascent, double* descent, double* line_gap) {
  if (!config_.font.normalize_font_list) return;
  
  *ascent *= (1.0 + GenerateNoise(config_.font.metrics_noise_range));
  *descent *= (1.0 + GenerateNoise(config_.font.metrics_noise_range));
  *line_gap *= (1.0 + GenerateNoise(config_.font.metrics_noise_range));
}

std::vector<std::string> FingerprintingProtectionService::FilterFontList(
    const std::vector<std::string>& fonts) {
  if (!config_.font.normalize_font_list) return fonts;
  
  std::vector<std::string> filtered;
  for (const auto& font : fonts) {
    if (std::find(config_.font.allowed_fonts.begin(),
                  config_.font.allowed_fonts.end(),
                  font) != config_.font.allowed_fonts.end()) {
      filtered.push_back(font);
    }
  }
  return filtered;
}

void FingerprintingProtectionService::ModifyAudioTimestamp(
    double* context_time, double* performance_time) {
  if (!config_.audio.modify_frequency_data) return;
  
  *context_time += GenerateNoise(config_.audio.timing_noise_range);
  *performance_time += GenerateNoise(config_.audio.timing_noise_range);
}

void FingerprintingProtectionService::ModifyFrequencyData(
    float* data, size_t length) {
  if (!config_.audio.modify_frequency_data) return;
  
  for (size_t i = 0; i < length; ++i) {
    data[i] *= (1.0f + static_cast<float>(
        GenerateNoise(config_.audio.frequency_noise_range)));
  }
}

uint32_t FingerprintingProtectionService::GetReportedCPUCores() const {
  return config_.hardware.reported_cpu_cores;
}

uint64_t FingerprintingProtectionService::GetReportedMemorySize() const {
  return config_.hardware.reported_memory_size;
}

double FingerprintingProtectionService::AddMemoryNoise(double value) const {
  if (!config_.memory.randomize_memory_values) return value;
  return value * (1.0 + GenerateNoise(config_.memory.memory_noise_range));
}

void FingerprintingProtectionService::SetConfig(const FingerprintingConfig& config) {
  config_ = config;
}

const FingerprintingConfig& FingerprintingProtectionService::GetConfig() const {
  return config_;
}

} // namespace ungoogled