#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CLIENT_HINTS_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CLIENT_HINTS_MANAGER_H_

#include "base/memory/ref_counted.h"
#include "net/http/http_request_headers.h"

namespace ungoogled {

class ClientHintsManager : public base::RefCountedThreadSafe<ClientHintsManager> {
 public:
  struct ClientHintsConfig {
    // Platform hints
    bool spoof_platform;
    bool spoof_architecture;
    bool spoof_model;
    
    // Browser hints
    bool modify_ua_full_version;
    bool modify_ua_mobile;
    bool modify_ua_platform;
    
    // Device hints
    bool spoof_device_memory;
    bool spoof_viewport_width;
    bool spoof_dpr;
    
    // Network hints
    bool modify_network_downlink;
    bool modify_network_rtt;
    bool modify_save_data;
  };

  bool Initialize(const ClientHintsConfig& config);
  bool ModifyClientHints(net::HttpRequestHeaders* headers);
  bool InterceptClientHintsAPI();

 private:
  bool GeneratePlatformHints();
  bool GenerateDeviceHints();
  bool GenerateNetworkHints();
  bool ValidateHintsCoherence();

  ClientHintsConfig config_;
  std::unique_ptr<ClientHintsInterceptor> interceptor_;
};

}  // namespace ungoogled

#endif