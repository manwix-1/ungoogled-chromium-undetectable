#ifndef COMPONENTS_UNGOOGLED_GRAPHICS_ADVANCED_GRAPHICS_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_GRAPHICS_ADVANCED_GRAPHICS_PROTECTION_H_

namespace ungoogled {

class AdvancedGraphicsProtection {
public:
    struct GraphicsConfig {
        struct CanvasConfig {
            bool noise_injection;         // Add subtle noise to canvas
            bool modify_readback;         // Modify canvas readback
            bool normalize_text;          // Standardize text rendering
            double noise_level;           // Noise intensity (0.0-1.0)
        };

        struct WebGLConfig {
            bool randomize_precision;     // Randomize shader precision
            bool modify_parameters;       // Modify WebGL parameters
            bool normalize_extensions;    // Standardize extensions
            std::vector<std::string> masked_extensions;
        };

        struct RenderingConfig {
            bool modify_color_depth;      // Modify color depth reporting
            bool normalize_dpi;           // Standardize DPI reporting
            bool randomize_viewport;      // Add viewport variations
            uint32_t viewport_noise;      // Viewport size variation
        };

        CanvasConfig canvas;
        WebGLConfig webgl;
        RenderingConfig rendering;
    };

    void Initialize(const GraphicsConfig& config);
    void ApplyProtection();

private:
    void ModifyCanvasFingerprint();
    void EnhanceWebGLProtection();
    void NormalizeRendering();

    GraphicsConfig config_;
    std::unique_ptr<CanvasModifier> canvas_modifier_;
    std::unique_ptr<WebGLProtector> webgl_protector_;
};

}  // namespace ungoogled

#endif