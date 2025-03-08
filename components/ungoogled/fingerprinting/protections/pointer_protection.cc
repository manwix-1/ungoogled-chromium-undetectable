#include "components/ungoogled/fingerprinting/protections/pointer_protection.h"

namespace ungoogled {

void PointerProtection::NormalizePointerEvent(PointerEvent* event) {
  if (!config_.normalize_pointer_events) {
    return;
  }

  // Add subtle noise to coordinates
  event->x += GenerateNoise(config_.coordinate_noise);
  event->y += GenerateNoise(config_.coordinate_noise);
  
  if (config_.standardize_pressure) {
    event->pressure = 0.5f; // Standard pressure
  }
  
  if (config_.normalize_tilt) {
    event->tiltX = 0;
    event->tiltY = 0;
  }
  
  // Normalize pointer properties
  event->width = 1.0f;
  event->height = 1.0f;
  event->isPrimary = true;
}

void PointerProtection::NormalizeTouchEvent(TouchEvent* event) {
  if (!config_.protect_touch_events) {
    return;
  }

  // Standardize touch list length
  while (event->touches.size() > 2) {
    event->touches.pop_back();
  }
  
  // Normalize each touch point
  for (Touch* touch : event->touches) {
    touch->radiusX = 11.0f;
    touch->radiusY = 11.0f;
    touch->rotationAngle = 0.0f;
    touch->force = 1.0f;
    
    // Add noise to coordinates
    touch->clientX += GenerateNoise(config_.coordinate_noise);
    touch->clientY += GenerateNoise(config_.coordinate_noise);
  }
}

} // namespace ungoogled