class InfiniteHardwareGenerator {
public:
    struct HardwareConfig {
        struct CPU {
            std::vector<int> core_counts = {2, 4, 6, 8, 12, 16};
            std::vector<std::string> architectures = {"x86_64", "arm64", "aarch64"};
            std::vector<std::string> vendors = {"GenuineIntel", "AuthenticAMD"};
        };

        struct GPU {
            std::vector<std::string> vendors;
            std::vector<std::string> renderers;
            std::vector<std::string> drivers;
            bool randomize_webgl = true;
        };

        struct Memory {
            std::vector<int> ram_sizes = {4, 8, 16, 32, 64};
            std::vector<int> heap_limits = {2048, 4096, 8192};
        };
    };

    std::string GenerateHardwareProfile();
private:
    HardwareConfig config_;
    std::unique_ptr<HardwareProfileLearner> profile_learner_;
};