class InfiniteFontGenerator {
public:
    struct FontConfig {
        struct SystemFonts {
            std::vector<std::string> common_fonts;
            std::vector<std::string> language_specific_fonts;
            std::map<std::string, std::vector<std::string>> regional_fonts;
        };

        struct Languages {
            std::vector<std::string> preferred_languages;
            std::vector<std::string> accept_languages;
            std::vector<std::string> encodings;
        };

        struct Typography {
            std::vector<double> rendering_options;
            std::vector<std::string> smoothing_techniques;
            bool randomize_metrics = true;
        };
    };

    std::string GenerateFontProfile();
private:
    FontConfig config_;
    std::unique_ptr<FontProfileLearner> font_learner_;
};