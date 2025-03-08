#ifndef COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_MANAGER_H_
#define COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_MANAGER_H_

#include "base/callback.h"
#include "components/ungoogled/fingerprinting/config.h"
#include "components/ungoogled/fingerprinting/protection_service.h"
#include <map>
#include <string>

namespace ungoogled {

// Forward declarations
class ProtectionProfile;
class DomainSettings;

class FingerprintingProtectionManager {
 public:
  enum class ProtectionLevel {
    DISABLED,
    BASIC,
    BALANCED,
    AGGRESSIVE,
    CUSTOM
  };

  enum class FeatureCategory {
    HARDWARE,
    NETWORK,
    BROWSER,
    MEDIA,
    CANVAS,
    WEBGL,
    FONTS,
    TIMING
  };

  using ProtectionCallback = base::RepeatingCallback<void(const std::string&)>;

  static FingerprintingProtectionManager* GetInstance();

  // Profile management
  void LoadProfile(const std::string& profile_name);
  void SaveProfile(const std::string& profile_name);
  void CreateCustomProfile(const std::string& name, const FingerprintingConfig& config);
  
  // Domain-specific settings
  void SetDomainProtectionLevel(const std::string& domain, ProtectionLevel level);
  void SetDomainFeatureOverride(const std::string& domain, 
                               FeatureCategory feature,
                               bool enabled);
  
  // Runtime protection management
  bool ShouldProtectDomain(const std::string& domain) const;
  bool IsFeatureProtected(const std::string& domain, FeatureCategory feature) const;
  
  // Monitoring and callbacks
  void AddProtectionCallback(const ProtectionCallback& callback);
  void RemoveProtectionCallback(const ProtectionCallback& callback);
  
  // Configuration
  void SetGlobalProtectionLevel(ProtectionLevel level);
  ProtectionLevel GetGlobalProtectionLevel() const;
  FingerprintingConfig GetEffectiveConfig(const std::string& domain) const;

 private:
  friend class base::NoDestructor<FingerprintingProtectionManager>;
  FingerprintingProtectionManager();
  ~FingerprintingProtectionManager();

  // Internal helpers
  void NotifyProtectionChange(const std::string& domain);
  FingerprintingConfig CreateConfigForLevel(ProtectionLevel level) const;
  bool ShouldApplyProtection(const std::string& domain, FeatureCategory feature) const;

  ProtectionLevel global_level_;
  std::map<std::string, std::unique_ptr<DomainSettings>> domain_settings_;
  std::map<std::string, std::unique_ptr<ProtectionProfile>> profiles_;
  std::vector<ProtectionCallback> callbacks_;
};

} // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_MANAGER_H_