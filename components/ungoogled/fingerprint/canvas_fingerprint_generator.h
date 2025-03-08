class InfiniteCanvasGenerator {
public:
    struct CanvasConfig {
        struct WebGL {
            std::vector<std::string> supported_extensions;
            std::vector<std::string> vendor_strings;
            std::vector<std::string> renderer_strings;
            bool randomize_parameters = true;
        };

        struct Drawing {
            std::vector<double> line_variations;
            std::vector<double> curve_qualities;
            std::vector<std::string> text_rendering_modes;
            bool add_noise_to_pixels = true;
        };

        struct Context {
            std::vector<std::string> color_spaces;
            std::vector<int> buffer_sizes;
            std::vector<std::string> blend_modes;
            bool randomize_image_data = true;
        };
    };

    std::string GenerateCanvasProfile();
private:
    CanvasConfig config_;
    std::unique_ptr<CanvasProfileLearner> canvas_learner_;
};