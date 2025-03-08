#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_TLS_FINGERPRINT_MODIFIER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_TLS_FINGERPRINT_MODIFIER_H_

#include "base/memory/ref_counted.h"
#include "net/ssl/ssl_config.h"
#include "net/socket/client_socket_handle.h"

namespace ungoogled {

class TLSFingerprintModifier : public base::RefCountedThreadSafe<TLSFingerprintModifier> {
 public:
  struct TLSConfig {
    // Client Hello customization
    struct {
      std::vector<uint16_t> cipher_suites;     // Allowed cipher suites
      std::vector<uint16_t> extensions;        // Enabled extensions
      std::vector<uint16_t> supported_groups;  // Supported groups
      std::vector<uint8_t> compression_methods; // Compression methods
      bool randomize_session_id;    // Randomize session ID
      bool modify_sni;             // Modify SNI behavior
    } client_hello;

    // Protocol behavior
    struct {
      bool randomize_record_size;   // Randomize TLS record size
      bool modify_padding;          // Modify padding behavior
      bool customize_alerts;        // Customize alert behavior
      bool vary_handshake;         // Vary handshake behavior
    } protocol;

    // Certificate handling
    struct {
      bool modify_validation;       // Modify cert validation
      bool customize_chain;         // Customize cert chain
      bool alter_key_usage;        // Alter key usage flags
      bool vary_signature;         // Vary signature algorithm
    } certificates;
  };

  bool Initialize(const TLSConfig& config);
  
  // Core TLS modifications
  void ModifySSLConfig(net::SSLConfig* config);
  void CustomizeClientHello(std::vector<uint8_t>* client_hello);
  void ModifyHandshake(net::ClientSocketHandle* socket);
  
  // Advanced features
  void RandomizeTLSRecords(std::vector<uint8_t>* record);
  void CustomizeCertValidation(net::X509Certificate* cert);
  void ModifyExtensions(std::vector<uint8_t>* extensions);

private:
  TLSConfig config_;
  std::unique_ptr<RandomNumberGenerator> rng_;
};

}  // namespace ungoogled

#endif