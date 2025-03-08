#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CONTROLLER_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_CONTROLLER_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/web_contents.h"

namespace ungoogled {

class AutomationController : public base::RefCountedThreadSafe<AutomationController> {
 public:
  struct InteractionConfig {
    double human_delay_min_ms;
    double human_delay_max_ms;
    bool simulate_input_events;
    bool randomize_patterns;
  };

  bool SimulateInteraction(content::WebContents* contents,
                          const InteractionConfig& config);
  
  bool ExecuteNavigation(const GURL& url, 
                        const NavigationConfig& config);
                        
  bool SimulateHumanBehavior(const BehaviorConfig& config);

 private:
  void RandomizeInteractionPatterns();
  void SimulateHumanTimings();
  void ApplyBehavioralFingerprint();
  
  std::unique_ptr<InputSimulator> input_simulator_;
  std::unique_ptr<BehaviorSimulator> behavior_simulator_;
};

}  // namespace ungoogled

#endif