#ifndef COMPONENTS_UNGOOGLED_TIMING_ADVANCED_TIMING_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_TIMING_ADVANCED_TIMING_PROTECTION_H_

namespace ungoogled {

class AdvancedTimingProtection {
public:
    struct TimingConfig {
        struct PerformanceConfig {
            bool modify_now;              // Modify performance.now()
            bool randomize_resolution;    // Randomize timing resolution
            bool add_execution_noise;     // Add execution time noise
            double noise_range;           // Timing noise range (ms)
        };

        struct HardwareConfig {
            bool modify_rdtsc;            // Modify RDTSC readings
            bool randomize_interrupts;    // Randomize interrupt timing
            bool normalize_latency;       // Standardize operation latency
            uint32_t latency_variation;   // Latency variation range
        };

        struct EventConfig {
            bool modify_event_timing;     // Modify event timestamps
            bool randomize_intervals;     // Randomize setInterval/setTimeout
            bool normalize_animation;     // Standardize requestAnimationFrame
            double interval_noise;        // Interval timing noise
        };

        PerformanceConfig performance;
        HardwareConfig hardware;
        EventConfig events;
    };

    void Initialize(const TimingConfig& config);
    void ApplyProtection();

private:
    void ModifyPerformanceTimings();
    void EnhanceHardwareTimings();
    void NormalizeEventTimings();

    TimingConfig config_;
    std::unique_ptr<TimingModifier> timing_modifier_;
    std::unique_ptr<HardwareTimer> hardware_timer_;
};

}  // namespace ungoogled

#endif
