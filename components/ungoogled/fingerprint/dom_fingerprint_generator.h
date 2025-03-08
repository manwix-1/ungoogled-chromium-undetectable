class InfiniteDOMGenerator {
public:
    struct DOMConfig {
        struct Layout {
            std::vector<std::string> supported_features;
            std::vector<std::string> css_properties;
            std::vector<double> layout_timings;
            bool randomize_offsets = true;
        };

        struct Elements {
            std::vector<std::string> custom_elements;
            std::vector<std::string> shadow_dom_features;
            std::vector<std::string> media_queries;
            bool modify_element_sizes = true;
        };

        struct Styles {
            std::vector<std::string> computed_styles;
            std::vector<std::string> pseudo_elements;
            std::vector<std::string> animations;
            bool randomize_style_inheritance = true;
        };
    };

    std::string GenerateDOMProfile();
private:
    DOMConfig config_;
    std::unique_ptr<DOMProfileLearner> dom_learner_;
};