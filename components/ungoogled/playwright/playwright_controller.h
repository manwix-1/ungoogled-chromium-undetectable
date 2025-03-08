#ifndef COMPONENTS_UNGOOGLED_PLAYWRIGHT_CONTROLLER_H_
#define COMPONENTS_UNGOOGLED_PLAYWRIGHT_CONTROLLER_H_

#include "base/memory/weak_ptr.h"
#include "components/ungoogled/playwright/virtual_display.h"
#include "components/ungoogled/core_automation/stealth_manager.h"
#include "components/ungoogled/core_automation/human_emulator.h"
#include "components/ungoogled/core_automation/fingerprint_manager.h"
#include "components/ungoogled/core_automation/network_obfuscator.h"

namespace ungoogled {

class PlaywrightController {
 public:
  struct AutomationConfig {
    StealthManager::StealthConfig stealth;
    HumanEmulator::BehaviorConfig behavior;
    FingerprintManager::CanvasConfig canvas;
    FingerprintManager::WebGLConfig webgl;
    FingerprintManager::NetworkConfig network;
    NetworkObfuscator::ShadowTLSConfig shadow_tls;
    NetworkObfuscator::ProxyChainConfig proxy_chain;
  };

  PlaywrightController();
  ~PlaywrightController();

  bool Initialize(const AutomationConfig& config);
  
  // Enhanced stealth methods
  bool NavigateStealthily(const std::string& url);
  bool HumanizedClick(int x, int y);
  bool NaturalType(const std::string& text);
  bool StealthyScreenshot(std::vector<uint8_t>* output);
  bool RotateFingerprint();
  bool SwitchProxy();

 private:
  scoped_refptr<VirtualDisplay> virtual_display_;
  scoped_refptr<StealthManager> stealth_manager_;
  scoped_refptr<FingerprintManager> fingerprint_manager_;
  scoped_refptr<NetworkObfuscator> network_obfuscator_;
  std::unique_ptr<HumanEmulator> human_emulator_;
  AutomationConfig config_;
  
  base::WeakPtrFactory<PlaywrightController> weak_factory_{this};
  
  DISALLOW_COPY_AND_ASSIGN(PlaywrightController);
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_PLAYWRIGHT_CONTROLLER_H_
