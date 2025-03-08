void Sensor::UpdateReading() {
  if (RuntimeEnabledFeatures::FingerprintingSensorProtection()) {
    auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
    auto config = protection_manager->GetEffectiveConfig(GetSecurityOrigin()->Domain());
    
    if (config.sensors.normalize_sensor_readings) {
      static thread_local std::random_device rd;
      static thread_local std::mt19937 gen(rd());
      std::normal_distribution<> noise(0.0, 0.01);
      
      auto NormalizeWithNoise = [&](double value) {
        return std::round((value + noise(gen)) * 100.0) / 100.0;
      };
      
      switch (type_) {
        case kTypeAccelerometer:
          reading_.accelerometer.x = NormalizeWithNoise(reading_.accelerometer.x);
          reading_.accelerometer.y = NormalizeWithNoise(reading_.accelerometer.y);
          reading_.accelerometer.z = NormalizeWithNoise(reading_.accelerometer.z);
          break;
          
        case kTypeGyroscope:
          reading_.gyroscope.x = NormalizeWithNoise(reading_.gyroscope.x);
          reading_.gyroscope.y = NormalizeWithNoise(reading_.gyroscope.y);
          reading_.gyroscope.z = NormalizeWithNoise(reading_.gyroscope.z);
          break;
          
        case kTypeMagnetometer:
          reading_.magnetometer.x = NormalizeWithNoise(reading_.magnetometer.x);
          reading_.magnetometer.y = NormalizeWithNoise(reading_.magnetometer.y);
          reading_.magnetometer.z = NormalizeWithNoise(reading_.magnetometer.z);
          break;
      }
    }
  }
  
  NotifyReading();
}