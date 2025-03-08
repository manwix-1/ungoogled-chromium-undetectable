#include "testing/gtest/include/gtest/gtest.h"
#include "components/ungoogled/core_automation/hardware_profiler.h"
#include "base/test/task_environment.h"
#include "base/values.h"

namespace ungoogled {
namespace {

class HardwareProfilerTest : public testing::Test {
 protected:
  void SetUp() override {
    profiler_ = std::make_unique<HardwareProfiler>();
  }

  std::unique_ptr<HardwareProfiler> profiler_;
  base::test::TaskEnvironment task_environment_;
};

TEST_F(HardwareProfilerTest, DefaultProfileGeneration) {
  HardwareProfile profile = profiler_->GenerateProfile();
  
  // Verify default values are within expected ranges
  EXPECT_GE(profile.max_texture_size, 2048);
  EXPECT_LE(profile.max_texture_size, 32768);
  
  EXPECT_GE(profile.sample_rate, 44000);
  EXPECT_LE(profile.sample_rate, 48000);
  
  EXPECT_GE(profile.screen_width, 800);
  EXPECT_LE(profile.screen_width, 7680);
  
  EXPECT_FALSE(profile.gpu_vendor.empty());
  EXPECT_FALSE(profile.gpu_renderer.empty());
  EXPECT_FALSE(profile.supported_extensions.empty());
}

TEST_F(HardwareProfilerTest, ProtectionLevels) {
  // Test different protection levels
  for (int level = 0; level <= 3; level++) {
    profiler_->SetFingerprintingProtectionLevel(level);
    
    auto webgl_info = profiler_->GetMaskedWebGLInfo();
    auto audio_info = profiler_->GetMaskedAudioCapabilities();
    
    if (level > 0) {
      // Protection enabled - should see masked values
      EXPECT_TRUE(webgl_info.FindStringKey("vendor"));
      EXPECT_TRUE(webgl_info.FindStringKey("renderer"));
      EXPECT_TRUE(webgl_info.FindKey("extensions"));
    }
  }
}

TEST_F(HardwareProfilerTest, ProfileValidation) {
  HardwareProfile invalid_profile;
  invalid_profile.max_texture_size = 1024;  // Too small
  invalid_profile.sample_rate = 96000;      // Too high
  invalid_profile.screen_width = 400;       // Too small
  
  EXPECT_FALSE(profiler_->ValidateProfile(invalid_profile));
  
  HardwareProfile valid_profile;
  valid_profile.max_texture_size = 4096;
  valid_profile.sample_rate = 44100;
  valid_profile.screen_width = 1920;
  valid_profile.screen_height = 1080;
  valid_profile.device_pixel_ratio = 1.0;
  
  EXPECT_TRUE(profiler_->ValidateProfile(valid_profile));
}

TEST_F(HardwareProfilerTest, ConsistentFingerprint) {
  // Enable protection
  profiler_->SetFingerprintingProtectionLevel(2);
  
  // Generate multiple profiles and verify consistency
  auto profile1 = profiler_->GenerateProfile();
  auto profile2 = profiler_->GenerateProfile();
  
  // Core identifiers should remain consistent
  EXPECT_EQ(profile1.gpu_vendor, profile2.gpu_vendor);
  EXPECT_EQ(profile1.gpu_renderer, profile2.gpu_renderer);
  
  // But some parameters should have controlled randomization
  EXPECT_NE(profile1.max_texture_size, profile2.max_texture_size);
  EXPECT_NE(profile1.sample_rate, profile2.sample_rate);
}

TEST_F(HardwareProfilerTest, WebGLMasking) {
  profiler_->SetFingerprintingProtectionLevel(2);
  
  auto webgl_info = profiler_->GetMaskedWebGLInfo();
  
  // Verify WebGL masking
  ASSERT_TRUE(webgl_info.is_dict());
  EXPECT_TRUE(webgl_info.FindStringKey("vendor"));
  EXPECT_TRUE(webgl_info.FindStringKey("renderer"));
  
  const base::Value* extensions = webgl_info.FindListKey("extensions");
  ASSERT_TRUE(extensions);
  EXPECT_FALSE(extensions->GetList().empty());
}

TEST_F(HardwareProfilerTest, AudioMasking) {
  profiler_->SetFingerprintingProtectionLevel(2);
  
  auto audio_info = profiler_->GetMaskedAudioCapabilities();
  
  // Verify audio masking
  ASSERT_TRUE(audio_info.is_dict());
  
  const base::Value* devices = audio_info.FindListKey("devices");
  ASSERT_TRUE(devices);
  EXPECT_FALSE(devices->GetList().empty());
  
  EXPECT_TRUE(audio_info.FindDoubleKey("sampleRate"));
  EXPECT_TRUE(audio_info.FindIntKey("channelCount"));
}

}  // namespace
}  // namespace ungoogled