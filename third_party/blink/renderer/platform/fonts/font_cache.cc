void FontCache::GetFontFamilyNames(HashSet<String>& family_names) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::FONTS)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    if (config.font.restrict_system_fonts) {
      // Return only allowed fonts
      for (const auto& font : config.font.allowed_fonts) {
        family_names.insert(String::FromUTF8(font.c_str()));
      }
      return;
    }
  }
  
  BaseGetFontFamilyNames(family_names);
}

scoped_refptr<SimpleFontData> FontCache::GetFontData(
    const FontDescription& font_description,
    const AtomicString& family_name) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  auto font_data = BaseGetFontData(font_description, family_name);
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::FONTS)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    if (config.font.spoof_metrics && font_data) {
      // Add noise to font metrics
      auto* platform_data = font_data->PlatformData();
      if (platform_data) {
        ModifyFontMetrics(platform_data, config.font.metrics_noise);
      }
    }
  }
  
  return font_data;
}