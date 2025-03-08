#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CONTROLLER_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CONTROLLER_MANAGER_H_

#include <memory>
#include "base/no_destructor.h"
#include "components/ungoogled/core_automation/fingerprint_orchestrator.h"
#include "components/ungoogled/core_automation/unified_protection.h"

namespace ungoogled {

class ControllerManager {
public:
    static ControllerManager* GetInstance();
    
    // Initialize all protection systems
    void InitializeControllers();
    
    // Configure protection levels
    void ConfigureProtectionLevel(ProtectionLevel level);
    
    // Access to individual controllers
    FingerprintOrchestrator* GetFingerprintController() { 
        return fingerprint_controller_.get(); 
    }
    
    UnifiedProtection* GetUnifiedProtection() { 
        return unified_protection_.get(); 
    }

private:
    friend class base::NoDestructor<ControllerManager>;
    ControllerManager();
    ~ControllerManager();

    void InitializeProtectionSystems();
    void ValidateControllerCoherence();

    std::unique_ptr<FingerprintOrchestrator> fingerprint_controller_;
    std::unique_ptr<UnifiedProtection> unified_protection_;
    std::unique_ptr<NetworkPrivacyManager> network_controller_;
};

} // namespace ungoogled
#endif  // COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CONTROLLER_MANAGER_H_