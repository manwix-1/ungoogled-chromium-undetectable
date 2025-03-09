class HardwareProtection {
public:
    void Initialize() {
        hardware_config_ = {
            .cpu_cores = {
                .mask_real_count = true,
                .randomize_value = true,
                .range = {4, 16}
            },
            .memory = {
                .mask_total_size = true,
                .randomize_heap_limits = true
            },
            .gpu = {
                .mask_vendor = true,
                .normalize_capabilities = true,
                .randomize_parameters = true
            }
        };
    }
};