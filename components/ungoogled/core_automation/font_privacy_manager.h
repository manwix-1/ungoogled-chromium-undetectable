#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_FONT_PRIVACY_MANAGER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_FONT_PRIVACY_MANAGER_H_

#include "base/memory/ref_counted.h"

namespace ungoogled {

class FontPrivacyManager : public base::RefCountedThreadSafe<FontPrivacyManager> {
 public:
  struct FontConfig {
    // Font Access
    bool restrict_system_fonts;
    bool use_generic_families;
    std::vector<std::string> allowed_fonts;
    
    // Font Metrics
    bool randomize_font_metrics;
    bool normalize_text_rendering;
    bool spoof_font_geometry;
    
    // Font Loading
    bool block_remote_fonts;
    bool cache_remote_fonts;
    bool sanitize_font_names;
    
    // Canvas Text
    bool protect_canvas_text;
    bool add_canvas_noise;
    float noise_level;
    
    // Font Fingerprinting
    bool prevent_font_enumeration;
    bool spoof_installed_fonts;
    bool randomize_font_order;
  };

  bool Initialize(const FontConfig& config);
  bool ApplyFontProtections();
  bool HandleFontRequest(const std::string& font_family);

 private:
  bool ConfigureFontAccess();
  bool SetupMetricsProtection();
  bool ManageFontLoading();
  bool ProtectCanvasText();
  bool PreventFingerprinting();

  FontConfig config_;
  std::unique_ptr<FontAccessController> font_controller_;
  std::unique_ptr<MetricsProtector> metrics_protector_;
  std::unique_ptr<FontLoader> font_loader_;
  std::unique_ptr<CanvasProtector> canvas_protector_;
};

}  // namespace ungoogled

#endif