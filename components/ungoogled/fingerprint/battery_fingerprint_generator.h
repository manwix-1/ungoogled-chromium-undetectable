class InfiniteBatteryGenerator {
public:
    struct BatteryConfig {
        struct Status {
            std::vector<double> charging_levels = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
            std::vector<bool> charging_states = {true, false};
            std::vector<double> discharge_times;
            std::vector<double> charge_times;
        };

        struct Behavior {
            bool randomize_status = true;
            bool simulate_charging = true;
            double update_interval = 60.0; // seconds
            bool mask_real_battery = true;
        };
    };

    std::string GenerateBatteryProfile();
private:
    BatteryConfig config_;
    std::unique_ptr<BatteryProfileLearner> battery_learner_;
};