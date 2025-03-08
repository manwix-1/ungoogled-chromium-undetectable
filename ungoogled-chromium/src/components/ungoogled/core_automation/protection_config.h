#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_PROTECTION_CONFIG_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_PROTECTION_CONFIG_H_

namespace ungoogled {

enum class ProtectionLevel {
    MINIMAL,
    STANDARD,
    ENHANCED,
    MAXIMUM
};

struct NoiseConfig {
    float canvas_noise_level;
    float webgl_noise_level;
    float audio_noise_level;
    float timing_variance;
};

struct EnhancedConfig {
    struct {
        bool disable_all;
    } client_hints;

    struct {
        bool block_fingerprinting;
    } brave_protections;

    struct {
        bool randomize_headers;
        bool spoof_timezone;
    } network_privacy;

    struct {
        bool randomize_math;
        bool normalize_timing;
    } js_privacy;

    bool enable_all_protections;
};

} // namespace ungoogled
#endif  // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_PROTECTION_CONFIG_H_