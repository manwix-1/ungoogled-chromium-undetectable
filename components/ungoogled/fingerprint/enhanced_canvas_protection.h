class EnhancedCanvasProtection {
public:
    struct CanvasConfig {
        struct NoiseConfig {
            double intensity = 0.02;
            std::string pattern = "perlin";
            double frequency = 0.1;
            bool adaptive_scaling = true;
        };

        struct TextConfig {
            bool normalize_rendering = true;
            bool randomize_metrics = true;
            double variance_px = 0.5;
        };
    };

    void ApplyProtection(const CanvasConfig& config) {
        canvas_protector_.Configure({
            .noise_generation = {
                .base_intensity = 0.02,
                .pattern_type = "perlin",
                .spatial_frequency = 0.1,
                .temporal_variance = 0.01
            },
            .text_protection = {
                .normalize_fonts = true,
                .randomize_spacing = true,
                .mask_metrics = true,
                .variance = 0.5
            },
            .color_protection = {
                .normalize_color_space = true,
                .add_color_noise = true,
                .mask_color_depth = true,
                .noise_amplitude = 0.02
            }
        });
    }
};