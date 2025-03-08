class InfiniteLocaleGenerator {
public:
    struct LocaleConfig {
        struct Language {
            std::vector<std::string> accepted_languages;
            std::vector<std::string> spellings;
            std::vector<std::string> dictionaries;
            bool randomize_order = true;
        };

        struct Region {
            std::vector<std::string> time_zones;
            std::vector<std::string> date_formats;
            std::vector<std::string> number_formats;
            bool consistent_formatting = true;
        };

        struct Input {
            std::vector<std::string> keyboard_layouts;
            std::vector<std::string> input_methods;
            std::vector<std::string> writing_scripts;
            bool simulate_typing_patterns = true;
        };
    };

    std::string GenerateLocaleProfile();
private:
    LocaleConfig config_;
    std::unique_ptr<LocaleProfileLearner> locale_learner_;
};