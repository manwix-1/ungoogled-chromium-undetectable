#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_EXTENSION_PRIVACY_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_EXTENSION_PRIVACY_MANAGER_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class ExtensionPrivacyManager : public base::RefCountedThreadSafe<ExtensionPrivacyManager> {
 public:
  struct ExtensionConfig {
    // Permission Controls
    bool restrict_file_access;
    bool block_network_access;
    bool limit_storage_access;
    bool prevent_background_running;
    
    // API Restrictions
    bool block_native_messaging;
    bool restrict_downloads_api;
    bool block_clipboard_access;
    bool prevent_extension_discovery;
    
    // Isolation
    bool sandbox_extensions;
    bool separate_extension_process;
    bool isolate_extension_storage;
    bool prevent_cross_extension_messaging;
    
    // Updates and Installation
    bool disable_auto_updates;
    bool verify_extension_signatures;
    bool block_external_extensions;
    bool allow_store_extensions_only;
  };

  bool Initialize(const ExtensionConfig& config);
  bool EnforceExtensionPolicy();
  bool ValidateExtension(const std::string& extension_id);

 private:
  bool ConfigurePermissions();
  bool RestrictAPIs();
  bool SetupIsolation();
  bool ManageUpdates();

  ExtensionConfig config_;
  std::unique_ptr<PermissionManager> permission_manager_;
  std::unique_ptr<APIController> api_controller_;
  std::unique_ptr<ExtensionIsolator> isolator_;
  std::unique_ptr<UpdateManager> update_manager_;
};

}  // namespace ungoogled

#endif