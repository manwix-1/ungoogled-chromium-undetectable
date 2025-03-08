class BrowserInstance {
 public:
  BrowserInstance(
      std::unique_ptr<CoreNetworkStack> network_stack,
      std::unique_ptr<VirtualDisplay> display,
      std::unique_ptr<AutomationEvader> evader,
      std::unique_ptr<VanadiumSecurity> vanadium,
      std::unique_ptr<FingerprintOrchestrator> fingerprint_orchestrator,
      std::unique_ptr<SensorProtection> sensor_protection,
      std::unique_ptr<StorageProtection> storage_protection,
      std::unique_ptr<WebRTCProtection> webrtc_protection);

  bool Initialize(const base::CommandLine& cmd_line);
  bool VerifyProtectionState() const;
  
  // Getters for all components
  CoreNetworkStack* GetNetworkStack() const { return network_stack_.get(); }
  VirtualDisplay* GetDisplay() const { return display_.get(); }
  AutomationEvader* GetEvader() const { return evader_.get(); }
  VanadiumSecurity* GetVanadium() const { return vanadium_.get(); }
  FingerprintOrchestrator* GetFingerprintOrchestrator() const { 
    return fingerprint_orchestrator_.get(); 
  }

  // Rotation methods
  bool RotateFingerprint();
  bool RotateProxy();
  bool RotateUserAgent();

 private:
  std::unique_ptr<CoreNetworkStack> network_stack_;
  std::unique_ptr<VirtualDisplay> display_;
  std::unique_ptr<AutomationEvader> evader_;
  std::unique_ptr<VanadiumSecurity> vanadium_;
  std::unique_ptr<FingerprintOrchestrator> fingerprint_orchestrator_;
  std::unique_ptr<SensorProtection> sensor_protection_;
  std::unique_ptr<StorageProtection> storage_protection_;
  std::unique_ptr<WebRTCProtection> webrtc_protection_;
};
