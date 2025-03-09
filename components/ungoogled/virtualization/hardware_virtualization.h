class HardwareVirtualization {
    struct VirtualizationConfig {
        struct CPUVirtualization {
            bool instruction_set_emulation;
            bool cache_behavior_simulation;
            bool frequency_scaling;
            struct Advanced {
                bool microarchitecture_spoofing;
                bool thermal_pattern_simulation;
                bool power_state_emulation;
            } advanced;
        };

        struct GPUVirtualization {
            bool render_path_virtualization;
            bool shader_simulation;
            bool memory_pattern_emulation;
            struct Parameters {
                bool vendor_specific_behavior;
                bool performance_characteristics;
                bool driver_signature_spoofing;
            } params;
        };
    };