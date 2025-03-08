#include "components/ungoogled/core_automation/noise_generator.h"
#include "base/time/time.h"
#include "base/rand_util.h"

namespace ungoogled {

NoiseGenerator::NoiseGenerator() 
    : rng_(std::make_unique<std::mt19937>(base::RandUint64())),
      dist_(std::make_unique<std::uniform_real_distribution<float>>(-1.0f, 1.0f)) {
}

void NoiseGenerator::ApplyToCanvas() {
    // Hook into Canvas2D and WebGL context creation
    content::RenderFrameHost::AllowInjectingJavaScript(
        "window.HTMLCanvasElement.prototype.getContext = (function() {"
        "  const originalGetContext = window.HTMLCanvasElement.prototype.getContext;"
        "  return function(contextType, contextAttributes) {"
        "    const ctx = originalGetContext.call(this, contextType, contextAttributes);"
        "    if (ctx && (contextType === '2d' || contextType.startsWith('webgl'))) {"
        "      const originalGetImageData = ctx.getImageData;"
        "      ctx.getImageData = function() {"
        "        const imageData = originalGetImageData.apply(this, arguments);"
        "        const pixels = imageData.data;"
        "        for (let i = 0; i < pixels.length; i += 4) {"
        "          pixels[i] = Math.floor(pixels[i] + (Math.random() - 0.5) * 2);"
        "          pixels[i+1] = Math.floor(pixels[i+1] + (Math.random() - 0.5) * 2);"
        "          pixels[i+2] = Math.floor(pixels[i+2] + (Math.random() - 0.5) * 2);"
        "        }"
        "        return imageData;"
        "      };"
        "    }"
        "    return ctx;"
        "  };"
        "})();");
}

void NoiseGenerator::ApplyToWebGL() {
    // Add noise to WebGL parameters and outputs
    content::RenderFrameHost::AllowInjectingJavaScript(
        "const originalGetParameter = WebGLRenderingContext.prototype.getParameter;"
        "WebGLRenderingContext.prototype.getParameter = function(pname) {"
        "  let result = originalGetParameter.call(this, pname);"
        "  if (typeof result === 'number') {"
        "    result += (Math.random() - 0.5) * 0.000001;"
        "  }"
        "  return result;"
        "};");
}

void NoiseGenerator::ApplyToAudio() {
    // Add subtle noise to audio processing
    content::RenderFrameHost::AllowInjectingJavaScript(
        "const originalCreateOscillator = AudioContext.prototype.createOscillator;"
        "AudioContext.prototype.createOscillator = function() {"
        "  const oscillator = originalCreateOscillator.call(this);"
        "  const originalFrequency = oscillator.frequency.value;"
        "  oscillator.frequency.value = originalFrequency + (Math.random() - 0.5) * 0.01;"
        "  return oscillator;"
        "};");
}

void NoiseGenerator::ApplyToTiming() {
    // Normalize high-resolution timing
    content::RenderFrameHost::AllowInjectingJavaScript(
        "const originalNow = Performance.prototype.now;"
        "Performance.prototype.now = function() {"
        "  const time = originalNow.call(this);"
        "  return Math.floor(time * 100) / 100;"
        "};");
}

float NoiseGenerator::GenerateNoise(float base_value, float noise_level) const {
    float clamped_noise = ClampNoiseLevel(noise_level, kMinNoiseLevel, kMaxNoiseLevel);
    float noise = (*dist_)(*rng_) * clamped_noise;
    return base_value * (1.0f + noise);
}

void NoiseGenerator::UpdateParameters(const NoiseConfig& config) {
    config_ = config;
}

bool NoiseGenerator::ValidateCanvasNoise() const {
    // Verify canvas noise is being applied correctly
    std::vector<uint8_t> test_data = {128, 128, 128, 255};
    auto noised_data = GeneratePixelNoise(test_data);
    
    for (size_t i = 0; i < test_data.size(); ++i) {
        if (!IsNoiseWithinTolerance(test_data[i], noised_data[i], kDefaultNoiseTolerance)) {
            return false;
        }
    }
    return true;
}

bool NoiseGenerator::ValidateWebGLNoise() const {
    // Verify WebGL parameter noise is being applied correctly
    float test_params[] = {1.0f, 1.0f, 1.0f};
    ApplyWebGLParameterNoise(test_params, 3);
    
    for (size_t i = 0; i < 3; ++i) {
        if (!IsNoiseWithinTolerance(1.0f, test_params[i], kDefaultNoiseTolerance)) {
            return false;
        }
    }
    return true;
}

float NoiseGenerator::ClampNoiseLevel(float value, float min_level, float max_level) const {
    return std::max(min_level, std::min(value, max_level));
}

bool NoiseGenerator::IsNoiseWithinTolerance(float original, float noised, float tolerance) const {
    return std::abs(original - noised) <= tolerance * std::abs(original);
}

} // namespace ungoogled