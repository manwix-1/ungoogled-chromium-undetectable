#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_FINGERPRINT_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_FINGERPRINT_H_

#include "base/memory/ref_counted.h"
#include "net/ssl/ssl_config.h"

namespace ungoogled {

class NetworkFingerprint : public base::RefCountedThreadSafe<NetworkFingerprint> {
 public:
  struct TLSConfig {
    std::string ja3_signature;
    std::vector<std::string> cipher_suites;
    std::vector<std::string> extensions;
    bool randomize_session_tickets;
    bool modify_client_hello;
  };

  struct TCPConfig {
    bool randomize_window_size;
    bool modify_mss;
    bool customize_tcp_options;
    bool spoof_tcp_timestamps;
  };

  struct ProxyConfig {
    std::vector<std::string> proxy_chain;
    bool rotate_residential_ips;
    bool avoid_datacenter_ips;
    bool validate_proxy_coherence;
  };

  bool ApplyTLSFingerprint(const TLSConfig& config);
  bool ConfigureTCPStack(const TCPConfig& config);
  bool SetupProxyChain(const ProxyConfig& config);
  
 private:
  bool ModifyTLSClientHello();
  bool CustomizeTCPStack();
  bool ValidateProxyChain();
  bool RotateResidentialIPs();
  
  std::unique_ptr<TLSManager> tls_manager_;
  std::unique_ptr<TCPController> tcp_controller_;
  std::unique_ptr<ProxyManager> proxy_manager_;
};

}  // namespace ungoogled

#endif