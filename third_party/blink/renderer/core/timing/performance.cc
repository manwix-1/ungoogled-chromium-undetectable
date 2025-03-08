DOMHighResTimeStamp Performance::now() const {
    auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
    std::string domain = GetSecurityOrigin()->Domain();
    
    DOMHighResTimeStamp time = BaseNow();
    
    if (protection_manager->IsFeatureProtected(domain, 
        ungoogled::FingerprintingProtectionManager::FeatureCategory::TIMING)) {
        
        auto config = protection_manager->GetEffectiveConfig(domain);
        
        // Add timing noise between -100ms to +100ms
        if (config.timing.add_noise) {
            static thread_local std::random_device rd;
            static thread_local std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-100.0, 100.0);
            time += dis(gen);
        }
        
        // Round to reduce precision (to nearest 100ms)
        if (config.timing.reduce_precision) {
            time = std::round(time / 100.0) * 100.0;
        }
    }
    
    return time;
}

void Performance::ModifyTimingValues(base::RepeatingCallback<double(double)> modifier) {
  // Modify all timing-related values
  navigation_timing_->ModifyValues(modifier);
  resource_timing_->ModifyValues(modifier);
  paint_timing_->ModifyValues(modifier);
}
