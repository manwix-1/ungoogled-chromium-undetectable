#ifndef COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_PROFILE_H_
#define COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_PROFILE_H_

#include "components/ungoogled/fingerprinting/config.h"
#include <string>

namespace ungoogled {

class ProtectionProfile {
 public:
  explicit ProtectionProfile(const std::string& name);

  // Profile management
  void SetConfig(const FingerprintingConfig& config);
  const FingerprintingConfig& GetConfig() const;
  
  // Metadata
  void SetDescription(const std::string& description);
  std::string GetDescription() const;
  std::string GetName() const;
  
  // Serialization
  base::Value::Dict ToDict() const;
  bool FromDict(const base::Value::Dict& dict);

 private:
  std::string name_;
  std::string description_;
  FingerprintingConfig config_;
};

} // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_PROFILE_H_