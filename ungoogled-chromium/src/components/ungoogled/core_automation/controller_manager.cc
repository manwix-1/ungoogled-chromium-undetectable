#include "components/ungoogled/core_automation/controller_manager.h"

namespace ungoogled {

ControllerManager* ControllerManager::GetInstance() {
    static base::NoDestructor<ControllerManager> instance;
    return instance.get();
}

ControllerManager::ControllerManager() {
    InitializeControllers();
}

void ControllerManager::InitializeControllers() {
    // Initialize core protection systems
    fingerprint_controller_ = std::make_unique<FingerprintOrchestrator>();
    unified_protection_ = std::make_unique<UnifiedProtection>();
    network_controller_ = std::make_unique<NetworkPrivacyManager>();

    InitializeProtectionSystems();
    ValidateControllerCoherence();
}

void ControllerManager::InitializeProtectionSystems() {
    // Configure base protection settings
    FingerprintOrchestrator::EnhancedConfig fp_config{
        .client_hints = {.disable_all = true},
        .brave_protections = {.block_fingerprinting = true},
        .network_privacy = {
            .randomize_headers = true,
            .spoof_timezone = true
        },
        .js_privacy = {
            .randomize_math = true,
            .normalize_timing = true
        },
        .enable_all_protections = true
    };

    // Initialize protection systems
    fingerprint_controller_->InitializeEnhancedProtections(fp_config);
    unified_protection_->InitializeUnifiedProtection();
    network_controller_->ConfigureNetworkPrivacy();
}

void ControllerManager::ValidateControllerCoherence() {
    // Validate system integrity
    fingerprint_controller_->ValidateProtectionCoherence();
    unified_protection_->ValidateProtections();
    network_controller_->ValidateConfiguration();
}

} // namespace ungoogled
