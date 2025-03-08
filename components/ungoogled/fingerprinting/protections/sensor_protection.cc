#include "components/ungoogled/fingerprinting/protections/sensor_protection.h"

namespace ungoogled {

SensorReadings SensorProtection::StandardizeSensorData(const SensorType type) {
  if (!config_.protect_sensors) {
    return GetRealSensorData(type);
  }

  SensorReadings readings;
  switch (type) {
    case SensorType::ACCELEROMETER:
      readings = {0.0, -9.81, 0.0}; // Standard gravity
      break;
    case SensorType::GYROSCOPE:
      readings = {0.0, 0.0, 0.0}; // No rotation
      break;
    case SensorType::MAGNETOMETER:
      readings = {0.0, 45.0, 0.0}; // Standard magnetic north
      break;
    case SensorType::AMBIENT_LIGHT:
      readings = {250.0}; // Standard indoor lighting
      break;
    case SensorType::PROXIMITY:
      readings = {100.0}; // Far distance
      break;
  }

  if (config_.add_sensor_noise) {
    AddControlledNoise(&readings, config_.sensor_noise_level);
  }

  return readings;
}

void SensorProtection::ProtectMotionSensors() {
  if (!config_.protect_motion) {
    return;
  }

  // Standardize device orientation
  device_orientation_ = {
    .alpha = 0.0,
    .beta = 0.0,
    .gamma = 0.0,
    .absolute = true
  };

  // Standardize device motion
  device_motion_ = {
    .acceleration = {0.0, 0.0, 0.0},
    .accelerationIncludingGravity = {0.0, -9.81, 0.0},
    .rotationRate = {0.0, 0.0, 0.0},
    .interval = 16.0
  };
}

bool SensorProtection::HandleSensorPermissionRequest(
    const SensorType type, const std::string& origin) {
  if (!config_.enforce_sensor_permissions) {
    return true;
  }

  // Check against whitelist
  if (IsOriginWhitelisted(origin)) {
    return true;
  }

  // Apply permission policy
  switch (config_.sensor_permission_policy) {
    case PermissionPolicy::PROMPT:
      return ShowPermissionPrompt(type, origin);
    case PermissionPolicy::DENY:
      return false;
    case PermissionPolicy::FAKE_DATA:
      EnableFakeDataMode(type);
      return true;
  }
  return false;
}

} // namespace ungoogled