class DeviceProxyHandler {
    struct DeviceConfig {
        // iOS specific
        struct IOSProxyConfig {
            bool vpn_profile_integration;
            bool mdm_compatibility;
            bool icloud_private_relay;
            struct SystemIntegration {
                bool system_wide_proxy;
                bool per_app_proxy;
                bool network_extension;
            } integration;
        };

        // Android specific
        struct AndroidProxyConfig {
            bool vpn_service;
            bool proxy_admin;
            bool work_profile_support;
            struct SystemFeatures {
                bool always_on_vpn;
                bool per_app_vpn;
                bool split_tunnel_vpn;
            } features;
        };
    };