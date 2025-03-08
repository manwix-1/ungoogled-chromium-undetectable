#ifndef COMPONENTS_UNGOOGLED_FINGERPRINTING_DOMAIN_SETTINGS_H_
#define COMPONENTS_UNGOOGLED_FINGERPRINTING_DOMAIN_SETTINGS_H_

#include "components/ungoogled/fingerprinting/protection_manager.h"
#include <map>
#include <string>

namespace ungoogled {

class DomainSettings {
 public:
  explicit DomainSettings(const std::string& domain);

  void SetProtectionLevel(FingerprintingProtectionManager::ProtectionLevel level);
  void SetFeatureOverride(FingerprintingProtectionManager::FeatureCategory feature,
                         bool enabled);
  
  FingerprintingProtectionManager::ProtectionLevel GetProtectionLevel() const;
  bool GetFeatureOverride(FingerprintingProtectionManager::FeatureCategory feature) const;
  bool HasFeatureOverride(FingerprintingProtectionManager::FeatureCategory feature) const;

  // Serialization
  base::Value::Dict ToDict() const;
  bool FromDict(const base::Value::Dict& dict);

 private:
  std::string domain_;
  FingerprintingProtectionManager::ProtectionLevel protection_level_;
  std::map<FingerprintingProtectionManager::FeatureCategory, bool> feature_overrides_;
};

} // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_FINGERPRINTING_DOMAIN_SETTINGS_H_