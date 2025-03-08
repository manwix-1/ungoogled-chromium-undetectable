class InfiniteStorageGenerator {
public:
    struct StorageConfig {
        struct LocalStorage {
            std::vector<int> quota_sizes;
            std::vector<std::string> persistence_types;
            std::vector<double> access_patterns;
            bool randomize_quotas = true;
        };

        struct Cache {
            std::vector<std::string> cache_modes;
            std::vector<int> cache_sizes;
            std::vector<double> ttl_values;
            bool simulate_network_conditions = true;
        };

        struct IndexedDB {
            std::vector<std::string> database_names;
            std::vector<int> object_store_counts;
            std::vector<std::string> transaction_modes;
            bool randomize_versions = true;
        };
    };

    std::string GenerateStorageProfile();
private:
    StorageConfig config_;
    std::unique_ptr<StorageProfileLearner> storage_learner_;
};