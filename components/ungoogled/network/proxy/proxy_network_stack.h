class ProxyNetworkStack {
    struct NetworkConfig {
        // Traffic routing
        struct TrafficRouting {
            bool split_tunneling;
            bool app_level_proxy;
            bool system_level_proxy;
            struct Rules {
                bool domain_routing;
                bool ip_routing;
                bool protocol_routing;
                std::vector<std::string> bypass_list;
            } routing_rules;
        };

        // Connection security
        struct SecurityLayer {
            bool ssl_pinning;
            bool dns_over_https;
            bool custom_dns_rules;
            struct Encryption {
                bool force_aes_256;
                bool perfect_forward_secrecy;
                bool certificate_verification;
            } encryption;
        };
    };