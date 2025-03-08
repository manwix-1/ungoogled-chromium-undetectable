void CanvasRenderingContext2D::FillRect(double x, double y, double width, double height) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::CANVAS)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    // Add subtle noise to coordinates and dimensions
    x += GenerateNoise(config.canvas.coordinate_noise);
    y += GenerateNoise(config.canvas.coordinate_noise);
    width += GenerateNoise(config.canvas.dimension_noise);
    height += GenerateNoise(config.canvas.dimension_noise);
  }
  
  BaseFillRect(x, y, width, height);
}

ImageData* CanvasRenderingContext2D::GetImageData(int sx, int sy, 
                                                 int sw, int sh,
                                                 ExceptionState& exception_state) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  ImageData* image_data = BaseGetImageData(sx, sy, sw, sh, exception_state);
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::CANVAS)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    // Add noise to pixel data
    uint8_t* data = image_data->data()->Data();
    for (int i = 0; i < image_data->data()->length(); i += 4) {
      data[i] += GeneratePixelNoise(config.canvas.pixel_noise);     // R
      data[i+1] += GeneratePixelNoise(config.canvas.pixel_noise);   // G
      data[i+2] += GeneratePixelNoise(config.canvas.pixel_noise);   // B
      // Leave alpha channel unchanged
    }
  }
  
  return image_data;
}