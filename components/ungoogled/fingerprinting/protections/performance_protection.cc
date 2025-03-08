#include "components/ungoogled/fingerprinting/protections/performance_protection.h"

namespace ungoogled {

DOMHighResTimeStamp PerformanceProtection::ProtectTimestamp(DOMHighResTimeStamp timestamp) {
  if (!config_.protect_timing_api) {
    return timestamp;
  }

  if (config_.quantize_timestamps) {
    // Round to configured resolution
    timestamp = std::round(timestamp / config_.timestamp_resolution_ms) 
               * config_.timestamp_resolution_ms;
  }
  
  // Add controlled noise
  timestamp += GenerateNoise(config_.timing_noise_range);
  
  return timestamp;
}

void PerformanceProtection::NormalizePerformanceEntry(PerformanceEntry* entry) {
  if (!config_.normalize_performance_entries) {
    return;
  }

  // Standardize common timing values
  entry->duration = ProtectTimestamp(entry->duration);
  entry->startTime = ProtectTimestamp(entry->startTime);
  
  if (entry->entryType == "resource") {
    auto* resource = static_cast<PerformanceResourceTiming*>(entry);
    
    // Normalize resource timing
    resource->connectStart = ProtectTimestamp(resource->connectStart);
    resource->connectEnd = ProtectTimestamp(resource->connectEnd);
    resource->domainLookupStart = ProtectTimestamp(resource->domainLookupStart);
    resource->domainLookupEnd = ProtectTimestamp(resource->domainLookupEnd);
    resource->redirectStart = ProtectTimestamp(resource->redirectStart);
    resource->redirectEnd = ProtectTimestamp(resource->redirectEnd);
    resource->requestStart = ProtectTimestamp(resource->requestStart);
    resource->responseStart = ProtectTimestamp(resource->responseStart);
    resource->responseEnd = ProtectTimestamp(resource->responseEnd);
    
    // Standardize transferred sizes
    resource->transferSize = StandardizeTransferSize(resource->transferSize);
    resource->encodedBodySize = StandardizeBodySize(resource->encodedBodySize);
    resource->decodedBodySize = StandardizeBodySize(resource->decodedBodySize);
  }
}

std::vector<PerformanceEntry*> PerformanceProtection::FilterEntries(
    const std::vector<PerformanceEntry*>& entries) {
  if (!config_.protect_resource_timing) {
    return entries;
  }

  std::vector<PerformanceEntry*> filtered;
  for (auto* entry : entries) {
    if (ShouldIncludeEntry(entry)) {
      NormalizePerformanceEntry(entry);
      filtered.push_back(entry);
    }
  }
  return filtered;
}

} // namespace ungoogled