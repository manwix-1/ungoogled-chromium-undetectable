#include "testing/gtest/include/gtest/gtest.h"
#include "components/ungoogled/core_automation/noise_generator.h"
#include "base/test/task_environment.h"
#include "base/time/time.h"

namespace ungoogled {
namespace {

class NoiseGeneratorTest : public testing::Test {
protected:
    void SetUp() override {
        noise_generator_ = std::make_unique<NoiseGenerator>();
        
        // Setup default noise configuration
        NoiseConfig default_config{
            .canvas_noise_level = 0.1f,
            .webgl_noise_level = 0.15f,
            .audio_noise_level = 0.05f,
            .timing_variance = 2.0f
        };
        noise_generator_->UpdateParameters(default_config);
    }

    // Helper methods
    bool IsWithinRange(float value, float expected, float tolerance) {
        return std::abs(value - expected) <= tolerance;
    }

    bool CheckNoiseDistribution(const std::vector<float>& samples, 
                              float expected_mean, 
                              float tolerance) {
        float sum = 0.0f;
        for (float sample : samples) {
            sum += sample;
        }
        float mean = sum / samples.size();
        return IsWithinRange(mean, expected_mean, tolerance);
    }

    std::unique_ptr<NoiseGenerator> noise_generator_;
    base::test::TaskEnvironment task_environment_;
};

TEST_F(NoiseGeneratorTest, CanvasNoiseGeneration) {
    // Test that canvas noise is within expected bounds
    std::vector<uint8_t> original_data = {128, 128, 128, 255};
    
    // Generate multiple samples to verify noise distribution
    const int kNumSamples = 1000;
    std::vector<float> noise_samples;
    
    for (int i = 0; i < kNumSamples; i++) {
        auto noised_data = noise_generator_->GeneratePixelNoise(original_data);
        ASSERT_EQ(noised_data.size(), original_data.size());
        
        // Check each color channel
        for (size_t j = 0; j < 3; j++) {  // Skip alpha channel
            float noise_ratio = static_cast<float>(noised_data[j]) / original_data[j];
            noise_samples.push_back(noise_ratio);
            
            // Verify noise is within acceptable range
            EXPECT_TRUE(IsWithinRange(noise_ratio, 1.0f, 0.2f));
        }
    }

    // Verify noise distribution is centered around 1.0
    EXPECT_TRUE(CheckNoiseDistribution(noise_samples, 1.0f, 0.01f));
}

TEST_F(NoiseGeneratorTest, WebGLParameterNoise) {
    const int kNumTestParams = 3;
    float test_params[kNumTestParams] = {1.0f, 1.0f, 1.0f};
    
    // Test multiple iterations
    const int kNumIterations = 1000;
    std::vector<float> noise_samples;
    
    for (int i = 0; i < kNumIterations; i++) {
        float params_copy[kNumTestParams];
        std::copy(test_params, test_params + kNumTestParams, params_copy);
        
        noise_generator_->ApplyWebGLParameterNoise(params_copy, kNumTestParams);
        
        for (int j = 0; j < kNumTestParams; j++) {
            float noise_ratio = params_copy[j] / test_params[j];
            noise_samples.push_back(noise_ratio);
            
            // Verify noise is within acceptable range
            EXPECT_TRUE(IsWithinRange(noise_ratio, 1.0f, 0.3f));
        }
    }
    
    // Verify noise distribution
    EXPECT_TRUE(CheckNoiseDistribution(noise_samples, 1.0f, 0.01f));
}

TEST_F(NoiseGeneratorTest, TimingNoise) {
    base::TimeDelta original_time = base::Milliseconds(100);
    
    // Test multiple iterations
    const int kNumIterations = 1000;
    std::vector<float> timing_ratios;
    
    for (int i = 0; i < kNumIterations; i++) {
        base::TimeDelta noised_time = noise_generator_->NormalizeTiming(original_time);
        float ratio = static_cast<float>(noised_time.InMicroseconds()) / 
                     original_time.InMicroseconds();
        timing_ratios.push_back(ratio);
        
        // Verify timing is within acceptable range
        EXPECT_TRUE(IsWithinRange(ratio, 1.0f, 0.1f));
    }
    
    // Verify timing noise distribution
    EXPECT_TRUE(CheckNoiseDistribution(timing_ratios, 1.0f, 0.01f));
}

TEST_F(NoiseGeneratorTest, NoiseConfigUpdate) {
    // Test that noise parameters are properly updated
    NoiseConfig new_config{
        .canvas_noise_level = 0.2f,
        .webgl_noise_level = 0.25f,
        .audio_noise_level = 0.1f,
        .timing_variance = 4.0f
    };
    
    noise_generator_->UpdateParameters(new_config);
    
    // Verify noise levels have changed
    std::vector<uint8_t> test_data = {128, 128, 128, 255};
    auto noised_data = noise_generator_->GeneratePixelNoise(test_data);
    
    // Higher noise level should result in larger variations
    float max_diff = 0.0f;
    for (size_t i = 0; i < 3; i++) {
        float diff = std::abs(static_cast<float>(noised_data[i]) - test_data[i]) / test_data[i];
        max_diff = std::max(max_diff, diff);
    }
    
    EXPECT_GT(max_diff, 0.15f);  // Should see larger variations with higher noise level
}

TEST_F(NoiseGeneratorTest, ValidationMethods) {
    // Test validation methods
    EXPECT_TRUE(noise_generator_->ValidateCanvasNoise());
    EXPECT_TRUE(noise_generator_->ValidateWebGLNoise());
    EXPECT_TRUE(noise_generator_->ValidateAudioNoise());
    EXPECT_TRUE(noise_generator_->ValidateTimingNoise());
    
    // Test with invalid config
    NoiseConfig invalid_config{
        .canvas_noise_level = 1.0f,  // Too high
        .webgl_noise_level = -0.1f,  // Invalid negative
        .audio_noise_level = 0.0f,   // Too low
        .timing_variance = 10.0f     // Too high
    };
    
    noise_generator_->UpdateParameters(invalid_config);
    
    // Validation should still pass due to internal clamping
    EXPECT_TRUE(noise_generator_->ValidateCanvasNoise());
    EXPECT_TRUE(noise_generator_->ValidateWebGLNoise());
}

}  // namespace
}  // namespace ungoogled