#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_FINGERPRINT_ORCHESTRATOR_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_FINGERPRINT_ORCHESTRATOR_H_

#include "base/memory/ref_counted.h"
#include "components/ungoogled/core_automation/client_hints_manager.h"
#include "components/ungoogled/core_automation/brave_fingerprint_protections.h"
#include "components/ungoogled/core_automation/vanadium_security.h"
#include "components/ungoogled/core_automation/network_privacy_manager.h"
#include "components/ungoogled/core_automation/storage_privacy_manager.h"
#include "components/ungoogled/core_automation/javascript_privacy_manager.h"
#include "components/ungoogled/core_automation/media_privacy_manager.h"
#include "components/ungoogled/core_automation/extension_privacy_manager.h"
#include "components/ungoogled/core_automation/font_privacy_manager.h"
#include "components/ungoogled/core_automation/webrtc_privacy_manager.h"
#include "components/ungoogled/core_automation/tls_fingerprint_manager.h"

namespace ungoogled {

class FingerprintOrchestrator : public base::RefCountedThreadSafe<FingerprintOrchestrator> {
 public:
  struct FingerprintProfile {
    // Hardware profile
    std::string cpu_architecture;
    int num_cores;
    int64_t memory_size;
    std::string gpu_vendor;
    std::string gpu_renderer;
    
    // OS and browser details
    std::string os_name;
    std::string os_version;
    std::string browser_version;
    std::string timezone;
    
    // Network characteristics
    std::string ip_address;
    std::string asn;
    bool is_mobile;
    std::string carrier;

    // Session persistence
    std::string session_id;
    bool maintain_session;
  };

  struct StabilityConfig {
    bool persist_fingerprint;
    int rotation_interval_hours;
    bool geo_lock;
    bool hardware_consistency;
  };

  bool InitializeProfile(const FingerprintProfile& profile,
                        const StabilityConfig& stability);
  
  bool RotateFingerprint(const RotationConstraints& constraints);
  
  bool ValidateConsistency();

  struct EnhancedConfig {
    ClientHintsManager::ClientHintsConfig client_hints;
    BraveFingerprintProtections::ProtectionConfig brave_protections;
    VanadiumSecurity::SecurityConfig vanadium_security;
    NetworkPrivacyManager::NetworkConfig network_privacy;
    StoragePrivacyManager::StorageConfig storage_privacy;
    JavaScriptPrivacyManager::JSConfig js_privacy;
    MediaPrivacyManager::MediaConfig media_privacy;
    ExtensionPrivacyManager::ExtensionConfig extension_privacy;
    FontPrivacyManager::FontConfig font_privacy;
    WebRTCPrivacyManager::WebRTCConfig webrtc_privacy;
    TLSFingerprintManager::TLSConfig tls_fingerprint;
    bool enable_all_protections;
  };

  bool InitializeEnhancedProtections(const EnhancedConfig& config);
  bool ValidateProtectionCoherence();
  
 private:
  bool GenerateHardwareProfile();
  bool AlignNetworkCharacteristics();
  bool SynchronizeTimezone();
  bool ValidateProfileCoherence();
  
  std::unique_ptr<FingerprintManager> fingerprint_manager_;
  std::unique_ptr<ProfilePersistence> profile_persistence_;
  bool SetupClientHints();
  bool ApplyBraveProtections();
  bool EnableVanadiumSecurity();
  bool ConfigureNetworkPrivacy();
  bool SetupStoragePrivacy();
  bool EnableJavaScriptProtections();
  
  std::unique_ptr<ClientHintsManager> client_hints_manager_;
  std::unique_ptr<BraveFingerprintProtections> brave_protections_;
  std::unique_ptr<VanadiumSecurity> vanadium_security_;
  std::unique_ptr<NetworkPrivacyManager> network_privacy_manager_;
  std::unique_ptr<StoragePrivacyManager> storage_privacy_manager_;
  std::unique_ptr<JavaScriptPrivacyManager> js_privacy_manager_;
  std::unique_ptr<MediaPrivacyManager> media_privacy_manager_;
  std::unique_ptr<ExtensionPrivacyManager> extension_privacy_manager_;
  std::unique_ptr<FontPrivacyManager> font_privacy_manager_;
  std::unique_ptr<WebRTCPrivacyManager> webrtc_privacy_manager_;
  std::unique_ptr<TLSFingerprintManager> tls_fingerprint_manager_;
};

}  // namespace ungoogled

#endif
