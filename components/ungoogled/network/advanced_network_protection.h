#ifndef COMPONENTS_UNGOOGLED_NETWORK_ADVANCED_NETWORK_PROTECTION_H_
#define COMPONENTS_UNGOOGLED_NETWORK_ADVANCED_NETWORK_PROTECTION_H_

namespace ungoogled {

class AdvancedNetworkProtection {
public:
    struct NetworkConfig {
        struct TCPConfig {
            bool randomize_window;        // Randomize TCP window size
            bool modify_timestamps;       // Modify TCP timestamps
            bool normalize_mss;           // Standardize MSS
            uint32_t window_variation;    // Window size variation range
        };

        struct TLSConfig {
            bool randomize_ciphersuites;  // Randomize cipher order
            bool modify_ja3_fingerprint;  // Modify JA3 fingerprint
            bool normalize_extensions;     // Standardize TLS extensions
            std::vector<std::string> allowed_ciphers;
        };

        struct DNSConfig {
            bool enforce_dns_over_https;  // Force DoH
            bool randomize_ttl;           // Randomize DNS TTL
            bool modify_query_patterns;   // Modify DNS query patterns
            std::vector<std::string> trusted_resolvers;
        };

        TCPConfig tcp;
        TLSConfig tls;
        DNSConfig dns;
    };

    void Initialize(const NetworkConfig& config);
    void ApplyProtection();

private:
    void ModifyTCPBehavior();
    void EnhanceTLSFingerprint();
    void ConfigureDNSProtection();

    NetworkConfig config_;
    std::unique_ptr<TCPStackModifier> tcp_modifier_;
    std::unique_unique<TLSFingerprintManager> tls_manager_;
};

}  // namespace ungoogled

#endif