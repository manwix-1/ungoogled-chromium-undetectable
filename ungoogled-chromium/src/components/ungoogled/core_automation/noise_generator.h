#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NOISE_GENERATOR_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NOISE_GENERATOR_H_

#include <random>
#include <memory>
#include "base/time/time.h"
#include "components/ungoogled/core_automation/protection_config.h"

namespace ungoogled {

class NoiseGenerator {
public:
    NoiseGenerator();
    ~NoiseGenerator();

    // Apply noise to different APIs
    void ApplyToCanvas();
    void ApplyToWebGL();
    void ApplyToAudio();
    void ApplyToTiming();

    // Configure noise parameters
    void UpdateParameters(const NoiseConfig& config);
    
    // Validation methods
    bool ValidateCanvasNoise() const;
    bool ValidateWebGLNoise() const;
    bool ValidateAudioNoise() const;
    bool ValidateTimingNoise() const;

private:
    // Noise generation methods
    float GenerateNoise(float base_value, float noise_level) const;
    std::vector<uint8_t> GeneratePixelNoise(const std::vector<uint8_t>& original_data) const;
    void ApplyWebGLParameterNoise(float* parameters, size_t count) const;
    
    // Timing normalization
    base::TimeDelta NormalizeTiming(base::TimeDelta original_time) const;
    
    // Internal utility methods
    float ClampNoiseLevel(float value, float min_level, float max_level) const;
    bool IsNoiseWithinTolerance(float original, float noised, float tolerance) const;

    // Random number generation
    std::unique_ptr<std::mt19937> rng_;
    std::unique_ptr<std::uniform_real_distribution<float>> dist_;
    
    // Configuration
    NoiseConfig config_;
    
    // Constants
    static constexpr float kMinNoiseLevel = 0.01f;
    static constexpr float kMaxNoiseLevel = 0.25f;
    static constexpr float kDefaultNoiseTolerance = 0.001f;
};

} // namespace ungoogled
#endif  // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NOISE_GENERATOR_H_