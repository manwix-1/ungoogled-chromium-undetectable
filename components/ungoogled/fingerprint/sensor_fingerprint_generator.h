class InfiniteSensorGenerator {
public:
    struct SensorConfig {
        struct Motion {
            std::vector<double> accelerometer_ranges;
            std::vector<double> gyroscope_ranges;
            std::vector<double> magnetometer_ranges;
            bool add_noise = true;
        };

        struct Environment {
            std::vector<double> light_levels;
            std::vector<double> proximity_values;
            std::vector<double> pressure_ranges;
            bool randomize_readings = true;
        };

        struct Orientation {
            std::vector<double> alpha_ranges;
            std::vector<double> beta_ranges;
            std::vector<double> gamma_ranges;
            bool simulate_movement = true;
        };
    };

    std::string GenerateSensorProfile();
private:
    SensorConfig config_;
    std::unique_ptr<SensorProfileLearner> sensor_learner_;
};