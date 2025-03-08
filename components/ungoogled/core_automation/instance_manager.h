#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_INSTANCE_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_INSTANCE_MANAGER_H_

#include "base/memory/weak_ptr.h"
#include "components/ungoogled/core_automation/network_stack.h"
#include "components/ungoogled/playwright/virtual_display.h"

namespace ungoogled {

class InstanceManager {
 public:
  struct InstanceConfig {
    ProxyConfig proxy;
    bool isolated_context;
    gfx::Size viewport;
    std::string user_agent;
  };

  InstanceManager();
  ~InstanceManager();

  // Instance lifecycle
  std::unique_ptr<BrowserInstance> CreateInstance(const InstanceConfig& config);
  void DestroyInstance(BrowserInstance* instance);
  
  // State management
  void PurgeAllState();
  bool VerifyInstanceIntegrity(BrowserInstance* instance);

 private:
  // Internal instance tracking
  std::map<BrowserInstance*, std::unique_ptr<CoreNetworkStack>> instance_networks_;
  std::map<BrowserInstance*, std::unique_ptr<VirtualDisplay>> instance_displays_;
  
  base::WeakPtrFactory<InstanceManager> weak_factory_{this};
  DISALLOW_COPY_AND_ASSIGN(InstanceManager);
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_INSTANCE_MANAGER_H_