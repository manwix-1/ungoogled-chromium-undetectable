#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_INSTANCE_ISOLATION_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_INSTANCE_ISOLATION_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/browser_context.h"

namespace ungoogled {

class InstanceIsolation : public base::RefCountedThreadSafe<InstanceIsolation> {
 public:
  struct IsolationConfig {
    bool isolate_storage;
    bool isolate_cache;
    bool isolate_cookies;
    bool isolate_permissions;
    std::string storage_prefix;
  };

  explicit InstanceIsolation(content::BrowserContext* context);
  
  bool ApplyIsolation(const IsolationConfig& config);
  void PurgeIsolatedState();
  
 private:
  bool SetupStoragePartition(const IsolationConfig& config);
  bool ConfigurePermissionContext(const IsolationConfig& config);
  bool InitializeIsolatedCache(const IsolationConfig& config);
  
  content::BrowserContext* context_;
  std::unique_ptr<StoragePartition> storage_partition_;
};

}  // namespace ungoogled

#endif