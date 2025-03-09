class MediaDevicesProtection {
public:
    void Initialize() {
        media_config_ = {
            .devices = {
                .mask_real_devices = true,
                .simulate_virtual_devices = true,
                .randomize_capabilities = true
            },
            .streams = {
                .add_noise = true,
                .normalize_parameters = true,
                .mask_real_resolution = true
            }
        };
    }
};