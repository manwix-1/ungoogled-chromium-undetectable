class ModelDatabase {
    struct DeviceProfiles {
        // iOS devices
        struct IOSDevices {
            std::vector<std::string> iphone_models;
            std::vector<std::string> ipad_models;
            std::vector<std::string> watch_models;
            struct ModelSpecifics {
                std::string soc_type;
                std::string gpu_core;
                std::string neural_engine;
            };
        };

        // Android devices
        struct AndroidDevices {
            std::vector<std::string> samsung_models;
            std::vector<std::string> pixel_models;
            std::vector<std::string> huawei_models;
            struct HardwareSpecs {
                std::string processor;
                std::string gpu_vendor;
                std::string sensor_suite;
            };
        };
    };