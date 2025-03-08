#include "components/ungoogled/fingerprinting/protections/crypto_protection.h"

namespace ungoogled {

void CryptoProtection::ProtectRandomValues(unsigned char* buffer, size_t length) {
  if (!config_.protect_random_values) {
    return;
  }

  // Use deterministic PRNG based on domain and session key
  std::seed_seq seed{domain_hash_, session_key_};
  std::mt19937 generator(seed);
  
  for (size_t i = 0; i < length; i++) {
    buffer[i] = static_cast<unsigned char>(generator() & 0xFF);
  }
}

std::string CryptoProtection::ProtectRandomUUID() {
  if (!config_.protect_random_uuid) {
    return GenerateStandardUUID();
  }
  
  // Generate deterministic UUID based on domain
  return GenerateDeterministicUUID(domain_hash_);
}

bool CryptoProtection::ShouldAllowAlgorithm(const std::string& algorithm) {
  if (!config_.standardize_crypto_capabilities) {
    return true;
  }
  
  static const std::set<std::string> kAllowedAlgorithms = {
    "AES-CBC",
    "AES-GCM",
    "HMAC",
    "RSA-OAEP",
    "ECDSA",
    "SHA-256"
  };
  
  return kAllowedAlgorithms.count(algorithm) > 0;
}

} // namespace ungoogled