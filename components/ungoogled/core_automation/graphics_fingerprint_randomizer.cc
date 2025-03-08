#include "components/ungoogled/core_automation/graphics_fingerprint_randomizer.h"
#include "base/rand_util.h"
#include "base/strings/string_util.h"
#include "gpu/command_buffer/service/context_state.h"

namespace ungoogled {

namespace {
// Pre-defined sets of vendor/renderer strings for rotation
const char* kVendorStrings[] = {
    "Mesa/X.org",
    "NVIDIA Corporation",
    "Intel Open Source Technology Center",
    "AMD Corporation"
};

const char* kRendererStrings[] = {
    "Mesa Intel(R) UHD Graphics 620 (KBL GT2)",
    "Mesa DRI Intel(R) Iris(R) Plus Graphics (ICL GT2)",
    "NVIDIA GeForce GTX 1060/PCIe/SSE2",
    "AMD Radeon RX 580"
};

// Noise patterns for canvas data
constexpr float kNoiseFactors[] = {0.01f, 0.02f, 0.015f, 0.025f};
}  // namespace

bool GraphicsFingerprintRandomizer::Initialize(const WebGLConfig& config) {
    config_ = config;
    rng_ = std::make_unique<base::RandomNumberGenerator>();
    return true;
}

void GraphicsFingerprintRandomizer::PatchWebGLContext(gl::GLContext* context) {
    if (!context || !config_.core.normalize_precision)
        return;

    // Normalize floating point precision
    context->SetWorkaroundsPlatform(gl::GLWorkarounds{
        .standardize_derivatives = true,
        .emulate_abs_int_function = true,
        .force_high_power_gpu = false,
        .force_gpu_mem_available_mb = 2048  // Standardized memory size
    });

    // Modify extension availability
    if (config_.core.standardize_extensions) {
        std::vector<std::string> standard_extensions = {
            "GL_EXT_texture_filter_anisotropic",
            "GL_OES_standard_derivatives",
            "GL_WEBGL_depth_texture"
        };
        context->SetAvailableExtensions(standard_extensions);
    }
}

void GraphicsFingerprintRandomizer::ModifyShaderSource(std::string* source) {
    if (!source || !config_.shaders.inject_noise)
        return;

    // Add controlled noise to shader precision
    static const char* precision_modifiers[] = {
        "lowp", "mediump", "highp"
    };

    std::string modified = *source;
    for (const char* modifier : precision_modifiers) {
        std::string search = std::string("precision ") + modifier;
        size_t pos = modified.find(search);
        if (pos != std::string::npos) {
            // Standardize precision to mediump
            modified.replace(pos, search.length(), "precision mediump");
        }
    }

    // Add subtle noise to floating point constants
    if (config_.shaders.inject_noise) {
        static const std::regex float_pattern(R"(\b\d+\.\d+\b)");
        modified = std::regex_replace(modified, float_pattern,
            [this](const std::smatch& m) {
                float value = std::stof(m.str());
                float noise = base::RandDouble() * 0.0001f;
                return std::to_string(value + noise);
            });
    }

    *source = modified;
}

void GraphicsFingerprintRandomizer::ApplyCanvasNoise(uint8_t* data, size_t length) {
    if (!data || !config_.parameters.randomize_max_values)
        return;

    // Apply subtle noise to canvas data
    for (size_t i = 0; i < length; i += 4) {
        float noise_factor = kNoiseFactors[rng_->RandInt(0, 3)];
        
        // Modify RGBA values with controlled randomization
        for (size_t j = 0; j < 4; j++) {
            float noise = base::RandDouble() * noise_factor;
            int value = data[i + j];
            value = std::clamp(static_cast<int>(value + (value * noise)), 0, 255);
            data[i + j] = static_cast<uint8_t>(value);
        }
    }
}

void GraphicsFingerprintRandomizer::NormalizeParameters(gpu::ContextState* state) {
    if (!state || !config_.parameters.modify_capabilities)
        return;

    // Standardize key WebGL parameters
    state->SetCapabilityState(GL_DEPTH_TEST, true);
    state->SetCapabilityState(GL_BLEND, true);
    state->SetCapabilityState(GL_CULL_FACE, true);

    // Normalize viewport and max values
    state->SetViewportState({
        .x = 0,
        .y = 0,
        .width = config_.core.max_texture_size,
        .height = config_.core.max_texture_size
    });

    // Standardize performance-related parameters
    if (config_.parameters.spoof_performance) {
        state->SetMaxTextureSize(config_.core.max_texture_size);
        state->SetMaxViewportDims({config_.core.max_texture_size,
                                 config_.core.max_texture_size});
        state->SetMaxRenderbufferSize(config_.core.max_texture_size);
    }
}

}  // namespace ungoogled