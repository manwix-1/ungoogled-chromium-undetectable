unsigned long Navigator::hardwareConcurrency() const {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::HARDWARE)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    if (config.hardware.spoof_hardware_concurrency) {
      // Return fixed number of cores
      return std::stoi(config.hardware.cpu_cores);
    }
  }
  
  return BaseHardwareConcurrency();
}

void Navigator::GetGamepads(Vector<Member<Gamepad>>& gamepads) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::HARDWARE)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    if (config.hardware.protect_gamepad) {
      // Return empty gamepad list
      gamepads.clear();
      return;
    }
  }
  
  BaseGetGamepads(gamepads);
}