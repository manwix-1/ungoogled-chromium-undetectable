#ifndef COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_SERVICE_H_
#define COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_SERVICE_H_

#include "base/memory/weak_ptr.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/ungoogled/fingerprinting/config.h"
#include <map>
#include <string>

namespace ungoogled {

class ProtectionService : public KeyedService {
 public:
  class Delegate {
   public:
    virtual void OnProtectionChanged(const std::string& domain) = 0;
    virtual void OnConfigurationChanged() = 0;
   protected:
    virtual ~Delegate() = default;
  };

  explicit ProtectionService(Delegate* delegate);
  ~ProtectionService() override;

  // Runtime protection
  void EnableProtection(const std::string& domain);
  void DisableProtection(const std::string& domain);
  bool IsProtectionEnabled(const std::string& domain) const;

  // Configuration management
  void SetConfig(const FingerprintingConfig& config);
  const FingerprintingConfig& GetConfig() const;
  void ResetToDefaults();

  // Domain management
  void AddProtectedDomain(const std::string& domain);
  void RemoveProtectedDomain(const std::string& domain);
  bool IsDomainProtected(const std::string& domain) const;

  // Feature-specific protection
  bool IsFeatureProtectedForDomain(const std::string& domain,
                                 const std::string& feature) const;
  void SetFeatureProtectionForDomain(const std::string& domain,
                                   const std::string& feature,
                                   bool enabled);

  // Utilities
  std::vector<std::string> GetProtectedDomains() const;
  base::WeakPtr<ProtectionService> GetWeakPtr();

 private:
  void NotifyProtectionChanged(const std::string& domain);
  void NotifyConfigurationChanged();

  Delegate* const delegate_;
  FingerprintingConfig config_;
  std::map<std::string, bool> protected_domains_;
  std::map<std::string, std::map<std::string, bool>> feature_overrides_;
  base::WeakPtrFactory<ProtectionService> weak_ptr_factory_{this};
};

} // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_FINGERPRINTING_PROTECTION_SERVICE_H_
