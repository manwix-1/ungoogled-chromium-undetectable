class InfiniteExtensionGenerator {
public:
    struct ExtensionConfig {
        struct Plugins {
            std::vector<std::string> mime_types;
            std::vector<std::string> plugin_names;
            std::vector<std::string> descriptions;
            bool randomize_versions = true;
        };

        struct Extensions {
            std::vector<std::string> extension_ids;
            std::vector<std::string> permissions;
            std::vector<std::string> content_scripts;
            bool mask_real_extensions = true;
        };

        struct Capabilities {
            std::vector<std::string> api_features;
            std::vector<std::string> protocols;
            std::vector<std::string> handlers;
            bool randomize_capabilities = true;
        };
    };

    std::string GenerateExtensionProfile();
private:
    ExtensionConfig config_;
    std::unique_ptr<ExtensionProfileLearner> extension_learner_;
};