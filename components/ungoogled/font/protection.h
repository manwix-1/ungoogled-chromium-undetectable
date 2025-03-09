class FontProtection {
public:
    void Initialize() {
        font_config_ = {
            .system_fonts_masking = true,
            .font_metrics_normalization = true,
            .rendering_randomization = {
                .enabled = true,
                .variation_range = 0.02
            },
            .font_list_protection = {
                .standardize_enumeration = true,
                .mask_custom_fonts = true
            }
        };
    }
};