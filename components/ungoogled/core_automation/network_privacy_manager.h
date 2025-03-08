#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_PRIVACY_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_PRIVACY_MANAGER_H_

#include "base/memory/ref_counted.h"
#include "net/http/http_request_headers.h"

namespace ungoogled {

class NetworkPrivacyManager : public base::RefCountedThreadSafe<NetworkPrivacyManager> {
 public:
  struct NetworkConfig {
    // DNS Privacy
    bool enable_dns_over_https;
    bool enable_dns_over_tls;
    std::string custom_dns_resolver;
    
    // Proxy Settings
    bool enable_socks_proxy;
    bool rotate_proxy_per_tab;
    std::string proxy_address;
    
    // Request Headers
    bool strip_tracking_headers;
    bool randomize_accept_language;
    bool spoof_referrer;
    
    // WebRTC Privacy
    bool disable_webrtc;
    bool force_webrtc_over_vpn;
    bool prevent_ip_leak;
  };

  bool Initialize(const NetworkConfig& config);
  bool ApplyNetworkPrivacy();
  bool ValidateNetworkSettings();

 private:
  bool ConfigureDNSPrivacy();
  bool SetupProxyRotation();
  bool ManageHeaders();
  bool SecureWebRTC();

  NetworkConfig config_;
  std::unique_ptr<DNSManager> dns_manager_;
  std::unique_ptr<ProxyRotator> proxy_rotator_;
  std::unique_ptr<HeaderManager> header_manager_;
};

}  // namespace ungoogled

#endif