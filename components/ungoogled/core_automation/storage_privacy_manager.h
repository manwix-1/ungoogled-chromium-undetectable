#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_STORAGE_PRIVACY_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_STORAGE_PRIVACY_MANAGER_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class StoragePrivacyManager : public base::RefCountedThreadSafe<StoragePrivacyManager> {
 public:
  struct StorageConfig {
    // Cookie Management
    bool block_third_party_cookies;
    bool clear_cookies_on_exit;
    bool partition_storage;
    
    // LocalStorage
    bool clear_local_storage;
    bool partition_local_storage;
    int storage_quota_mb;
    
    // Cache Management
    bool disable_cache;
    bool clear_cache_on_exit;
    bool partition_cache_by_domain;
    
    // IndexedDB
    bool block_indexeddb;
    bool clear_indexeddb_on_exit;
    
    // Other Storage
    bool block_file_system_api;
    bool block_shared_workers;
    bool block_service_workers;
  };

  bool Initialize(const StorageConfig& config);
  bool EnforceStoragePolicy();
  bool ClearStorageData(const std::string& domain = "");

 private:
  bool ManageCookies();
  bool ConfigureLocalStorage();
  bool SetupCacheControls();
  bool RestrictDatabaseAccess();

  StorageConfig config_;
  std::unique_ptr<CookieManager> cookie_manager_;
  std::unique_ptr<StorageQuotaManager> quota_manager_;
  std::unique_ptr<CacheController> cache_controller_;
};

}  // namespace ungoogled

#endif