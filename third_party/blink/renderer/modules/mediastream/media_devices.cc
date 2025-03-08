void MediaDevices::EnumerateDevices(ScriptPromiseResolver* resolver) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::MEDIA)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    if (config.media.protect_media_devices) {
      // Return minimal generic device info
      Vector<MediaDeviceInfo> devices;
      
      // Generic audio input
      if (config.media.spoof_media_capabilities) {
        devices.push_back(CreateGenericDevice(
            "default-audio-input",
            MediaDeviceInfo::MediaDeviceKind::kAudioInput,
            "Default Audio Input"));
      }
      
      // Generic audio output
      if (config.media.spoof_media_capabilities) {
        devices.push_back(CreateGenericDevice(
            "default-audio-output",
            MediaDeviceInfo::MediaDeviceKind::kAudioOutput,
            "Default Audio Output"));
      }
      
      // Generic video input
      if (config.media.spoof_media_capabilities) {
        devices.push_back(CreateGenericDevice(
            "default-video-input",
            MediaDeviceInfo::MediaDeviceKind::kVideoInput,
            "Default Video Input"));
      }
      
      resolver->Resolve(devices);
      return;
    }
  }
  
  BaseEnumerateDevices(resolver);
}

void MediaDevices::GetSupportedConstraints(MediaTrackSupportedConstraints& supported_constraints) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::MEDIA)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    if (config.media.spoof_media_capabilities) {
      // Return minimal capabilities
      supported_constraints.setEchoCancellation(true);
      supported_constraints.setNoiseSuppression(true);
      supported_constraints.setAutoGainControl(true);
      supported_constraints.setChannelCount(true);
      supported_constraints.setSampleRate(true);
      supported_constraints.setSampleSize(true);
      return;
    }
  }
  
  BaseGetSupportedConstraints(supported_constraints);
}