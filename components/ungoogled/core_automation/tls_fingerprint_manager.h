#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_TLS_FINGERPRINT_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_TLS_FINGERPRINT_MANAGER_H_

#include "base/memory/ref_counted.h"
#include "net/ssl/ssl_config.h"

namespace ungoogled {

class TLSFingerprintManager : public base::RefCountedThreadSafe<TLSFingerprintManager> {
 public:
  struct TLSConfig {
    // TLS Version Control
    bool force_tls_version;
    uint16_t min_version;
    uint16_t max_version;
    
    // Cipher Suite Management
    std::vector<uint16_t> allowed_cipher_suites;
    bool randomize_cipher_order;
    bool enforce_secure_ciphers;
    
    // Extension Control
    std::vector<uint16_t> enabled_extensions;
    std::vector<uint16_t> disabled_extensions;
    bool randomize_extensions;
    
    // Client Hello Customization
    bool modify_client_hello;
    std::string custom_session_id;
    std::vector<uint8_t> custom_compression_methods;
    
    // Certificate Handling
    bool custom_cert_verification;
    bool spoof_cert_chain;
    bool modify_cert_fingerprint;
    
    // JA3 Fingerprint
    bool spoof_ja3_fingerprint;
    std::string forced_ja3_fingerprint;
    bool randomize_ja3;
    
    // Advanced Features
    bool enable_utls;
    std::string utls_browser_profile;
    bool modify_alpn_protocols;
    std::vector<std::string> custom_alpn_protocols;
  };

  bool Initialize(const TLSConfig& config);
  bool ApplyTLSProtections();
  bool ModifySSLConfig(net::SSLConfig* config);
  bool InterceptClientHello(std::vector<uint8_t>* client_hello);
  bool SpoofJA3Fingerprint();
  bool CustomizeCertificateChain();

 private:
  bool SetupTLSVersion();
  bool ConfigureCipherSuites();
  bool ManageExtensions();
  bool CustomizeClientHello();
  bool SetupCertHandling();
  bool ConfigureJA3Spoofing();
  
  TLSConfig config_;
  std::unique_ptr<ClientHelloModifier> hello_modifier_;
  std::unique_ptr<CipherSuiteManager> cipher_manager_;
  std::unique_ptr<ExtensionController> extension_controller_;
  std::unique_ptr<CertificateSpoofer> cert_spoofer_;
  std::unique_ptr<JA3Generator> ja3_generator_;
  std::unique_ptr<UTLSManager> utls_manager_;
};

}  // namespace ungoogled

#endif