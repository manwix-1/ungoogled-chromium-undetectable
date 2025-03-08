bool PlaywrightController::Initialize(const AutomationConfig& config) {
  // Initialize launch protocol
  launch_protocol_ = std::make_unique<LaunchProtocol>();

  // Initialize stealth manager with config
  stealth_manager_ = std::make_unique<StealthManager>();
  if (!stealth_manager_->Initialize(config.stealth)) {
    return false;
  }

  // Initialize human emulator
  human_emulator_ = std::make_unique<HumanEmulator>();
  if (!human_emulator_->Initialize(config.behavior)) {
    return false;
  }

  // Initialize fingerprint manager
  fingerprint_manager_ = std::make_unique<FingerprintManager>();
  if (!fingerprint_manager_->Initialize(config.canvas, 
                                      config.webgl,
                                      config.network)) {
    return false;
  }

  // Initialize network obfuscator
  network_obfuscator_ = std::make_unique<NetworkObfuscator>();
  if (!network_obfuscator_->Initialize(config.shadow_tls,
                                     config.proxy_chain)) {
    return false;
  }

  return true;
}