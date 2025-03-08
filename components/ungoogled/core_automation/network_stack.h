#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_STACK_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_STACK_H_

#include "base/memory/ref_counted.h"
#include "net/proxy_resolution/proxy_config.h"
#include "net/url_request/url_request_context.h"

namespace ungoogled {

class CoreNetworkStack : public base::RefCountedThreadSafe<CoreNetworkStack> {
 public:
  struct ProxyConfig {
    std::string host;
    uint16_t port;
    std::string username;
    std::string password;
    bool direct_fallback;
  };

  CoreNetworkStack();
  
  // Core proxy management
  bool InitializeProxy(const ProxyConfig& config);
  bool RotateProxy(const ProxyConfig& new_config);
  void ResetProxyState();

  // Connection management
  std::unique_ptr<net::URLRequestContext> CreateRequestContext();
  void PurgeConnectionPool();

 private:
  friend class base::RefCountedThreadSafe<CoreNetworkStack>;
  ~CoreNetworkStack();

  // Internal proxy state
  net::ProxyConfig current_proxy_;
  std::unique_ptr<net::URLRequestContext> base_context_;
  
  DISALLOW_COPY_AND_ASSIGN(CoreNetworkStack);
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_NETWORK_STACK_H_