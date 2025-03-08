class InfiniteScreenGenerator {
public:
    struct ScreenConfig {
        struct Resolution {
            std::vector<int> widths = {1366, 1920, 2560, 3840};
            std::vector<int> heights = {768, 1080, 1440, 2160};
            double aspect_ratio_tolerance = 0.1;
        };

        struct ColorProfile {
            std::vector<int> color_depths = {24, 30, 32};
            std::vector<int> pixel_depths = {24, 32, 48};
            std::vector<std::string> color_gamuts = {"srgb", "p3", "rec2020"};
        };

        struct MediaQueries {
            std::vector<std::string> orientations = {"portrait", "landscape"};
            std::vector<double> pixel_ratios = {1.0, 1.5, 2.0, 2.5, 3.0};
        };
    };

    std::string GenerateScreenFingerprint();
private:
    ScreenConfig config_;
    std::unique_ptr<MarkovChainGenerator> resolution_generator_;
    std::unique_ptr<ColorProfileLearner> color_learner_;
};