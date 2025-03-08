#include "components/ungoogled/fingerprinting/protection_service.h"

void DOMWindow::performance(Performance& performance) {
  auto* protection_service = ungoogled::FingerprintingProtectionService::GetInstance();
  
  // Modify timing-related values
  if (protection_service->GetConfig().memory.normalize_performance_values) {
    performance.ModifyTimingValues([protection_service](double value) {
      return protection_service->AddMemoryNoise(value);
    });
  }
}

unsigned long DOMWindow::hardwareConcurrency() const {
  return ungoogled::FingerprintingProtectionService::GetInstance()
      ->GetReportedCPUCores();
}