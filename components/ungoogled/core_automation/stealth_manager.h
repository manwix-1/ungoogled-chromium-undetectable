#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_STEALTH_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_STEALTH_MANAGER_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/web_contents.h"

namespace ungoogled {

class StealthManager : public base::RefCountedThreadSafe<StealthManager> {
 public:
  struct StealthConfig {
    // Navigator Properties
    bool spoof_languages;
    bool modify_plugins;
    bool randomize_vendor;
    std::vector<std::string> custom_languages;
    std::string custom_vendor;
    
    // WebGL Fingerprinting
    bool mask_webgl_info;
    bool randomize_webgl_params;
    bool spoof_webgl_vendor;
    std::string forced_webgl_vendor;
    
    // Hardware Concurrency
    bool modify_hardware_concurrency;
    int custom_hardware_concurrency;
    
    // Chrome Runtime
    bool hide_chrome_runtime;
    bool mask_automation_flags;
    bool disable_automation_props;
    
    // Permission Handling
    bool auto_grant_permissions;
    std::vector<std::string> allowed_permissions;
    
    // Browser Behavior
    bool emulate_real_mouse;
    bool simulate_human_timing;
    bool randomize_viewport;
    
    // Advanced Evasions
    bool enable_stealth_scripts;
    bool inject_stealth_css;
    bool patch_prototype_methods;
    bool fake_ui_interactions;
    
    // Automation Framework
    bool hide_webdriver;
    bool mask_playwright;
    bool spoof_permissions;
    bool randomize_useragent;
  };

  bool Initialize(const StealthConfig& config);
  bool ApplyStealthProtections(content::WebContents* contents);
  bool InjectStealthScripts(content::RenderFrameHost* frame);
  bool PatchNavigator(content::RenderFrameHost* frame);
  bool ModifyWebGLContext(content::RenderFrameHost* frame);
  bool SetupHumanBehavior();

 private:
  bool SetupNavigatorSpoofing();
  bool ConfigureWebGLMasking();
  bool ManagePermissions();
  bool SetupAutomationEvasion();
  bool InjectStealthCSS();
  bool PatchPrototypes();
  
  StealthConfig config_;
  std::unique_ptr<NavigatorSpoofer> navigator_spoofer_;
  std::unique_ptr<WebGLMasker> webgl_masker_;
  std::unique_ptr<PermissionManager> permission_manager_;
  std::unique_ptr<HumanEmulator> human_emulator_;
  std::unique_ptr<StealthScriptInjector> script_injector_;
  std::unique_ptr<AutomationEvader> automation_evader_;
};

}  // namespace ungoogled

#endif