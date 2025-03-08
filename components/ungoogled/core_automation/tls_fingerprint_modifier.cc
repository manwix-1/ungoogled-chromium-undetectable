#include "components/ungoogled/core_automation/tls_fingerprint_modifier.h"
#include "base/rand_util.h"
#include "net/ssl/ssl_cipher_suite_names.h"

namespace ungoogled {

namespace {
// Standard cipher suites for rotation
const uint16_t kStandardCipherSuites[] = {
    0xC02B,  // TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    0xC02F,  // TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
    0xC02C,  // TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
    0xC030   // TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
};

// Standard TLS extensions
const uint16_t kStandardExtensions[] = {
    0x0000,  // server_name
    0x000A,  // supported_groups
    0x000B,  // ec_point_formats
    0x000D,  // signature_algorithms
    0x0017   // extended_master_secret
};
}  // namespace

bool TLSFingerprintModifier::Initialize(const TLSConfig& config) {
    config_ = config;
    rng_ = std::make_unique<base::RandomNumberGenerator>();
    return true;
}

void TLSFingerprintModifier::ModifySSLConfig(net::SSLConfig* config) {
    if (!config || !config_.client_hello.randomize_session_id)
        return;

    // Modify cipher suites
    if (!config_.client_hello.cipher_suites.empty()) {
        config->cipher_suite_preferences = config_.client_hello.cipher_suites;
    } else {
        // Use standard rotation
        std::vector<uint16_t> suites(std::begin(kStandardCipherSuites),
                                   std::end(kStandardCipherSuites));
        std::shuffle(suites.begin(), suites.end(), 
                    std::default_random_engine(base::RandUint64()));
        config->cipher_suite_preferences = suites;
    }

    // Modify TLS extensions
    if (config_.client_hello.modify_sni) {
        config->alpn_protos.clear();  // Disable ALPN
        // Add standard extensions
        for (uint16_t ext : kStandardExtensions) {
            config->enabled_tls_extensions.push_back(ext);
        }
    }

    // Customize session behavior
    if (config_.protocol.randomize_record_size) {
        config->record_size_limit = base::RandInt(512, 16384);
    }
}

void TLSFingerprintModifier::CustomizeClientHello(
    std::vector<uint8_t>* client_hello) {
    if (!client_hello || !config_.client_hello.randomize_session_id)
        return;

    // Generate random session ID
    std::vector<uint8_t> session_id(32);
    base::RandBytes(session_id.data(), session_id.size());

    // Modify compression methods
    if (!config_.client_hello.compression_methods.empty()) {
        size_t comp_methods_offset = FindCompressionMethodsOffset(*client_hello);
        if (comp_methods_offset != std::string::npos) {
            std::copy(config_.client_hello.compression_methods.begin(),
                     config_.client_hello.compression_methods.end(),
                     client_hello->begin() + comp_methods_offset);
        }
    }
}

void TLSFingerprintModifier::ModifyHandshake(
    net::ClientSocketHandle* socket) {
    if (!socket || !config_.protocol.vary_handshake)
        return;

    // Modify handshake timing
    if (config_.protocol.randomize_record_size) {
        base::TimeDelta delay = base::Milliseconds(base::RandInt(1, 5));
        socket->SetHandshakeDelay(delay);
    }

    // Customize alert behavior
    if (config_.protocol.customize_alerts) {
        socket->SetAlertCallback(base::BindRepeating(
            &TLSFingerprintModifier::HandleAlert, base::Unretained(this)));
    }
}

void TLSFingerprintModifier::CustomizeCertValidation(
    net::X509Certificate* cert) {
    if (!cert || !config_.certificates.modify_validation)
        return;

    // Modify certificate chain validation
    if (config_.certificates.customize_chain) {
        std::vector<std::string> allowed_names = {
            "DigiCert Global Root CA",
            "GlobalSign Root CA",
            "Sectigo Root CA"
        };
        cert->SetAllowedIssuerNames(allowed_names);
    }

    // Modify key usage flags
    if (config_.certificates.alter_key_usage) {
        cert->SetKeyUsageConstraints(
            net::KEY_USAGE_DIGITAL_SIGNATURE |
            net::KEY_USAGE_KEY_ENCIPHERMENT);
    }
}

}  // namespace ungoogled