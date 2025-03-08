class InfiniteBehaviorGenerator {
public:
    struct BehaviorConfig {
        struct Input {
            double min_typing_speed = 100.0;
            double max_typing_speed = 500.0;
            std::vector<double> mouse_movement_patterns;
            std::vector<double> scroll_patterns;
        };

        struct Timing {
            double min_reaction_time = 50.0;
            double max_reaction_time = 300.0;
            std::vector<double> event_delays;
            std::vector<double> focus_durations;
        };

        struct Navigation {
            std::vector<std::string> common_patterns;
            std::vector<double> page_view_durations;
            std::vector<std::string> click_patterns;
        };
    };

    std::string GenerateBehaviorPattern();
private:
    BehaviorConfig config_;
    std::unique_ptr<BehaviorLearner> behavior_learner_;
    std::unique_ptr<MarkovChainGenerator> pattern_generator_;
};