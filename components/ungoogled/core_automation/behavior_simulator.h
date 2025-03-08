#ifndef COMPONENTS_UNGOOGLED_CORE_AUTOMATION_BEHAVIOR_SIMULATOR_H_
#define COMPONENTS_UNGOOGLED_CORE_AUTOMATION_BEHAVIOR_SIMULATOR_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/web_contents.h"

namespace ungoogled {

class BehaviorSimulator : public base::RefCountedThreadSafe<BehaviorSimulator> {
 public:
  struct MouseConfig {
    bool natural_movement;
    bool bezier_curves;
    double speed_variation;
    bool simulate_acceleration;
    bool add_human_error;
  };

  struct KeyboardConfig {
    bool variable_typing_speed;
    bool natural_mistakes;
    bool language_specific;
    bool simulate_shortcuts;
  };

  struct InteractionConfig {
    bool natural_scrolling;
    bool realistic_focus;
    bool simulate_attention;
    bool random_exploration;
  };

  bool SimulateMouseBehavior(content::WebContents* contents,
                            const MouseConfig& config);
  bool SimulateKeyboardInput(const KeyboardConfig& config);
  bool SimulatePageInteraction(const InteractionConfig& config);
  
 private:
  bool GenerateMousePath();
  bool SimulateTypingPatterns();
  bool EmulateHumanAttention();
  bool CreateInteractionSequence();
  
  std::unique_ptr<MouseSimulator> mouse_simulator_;
  std::unique_ptr<KeyboardSimulator> keyboard_simulator_;
  std::unique_ptr<InteractionSimulator> interaction_simulator_;
};

}  // namespace ungoogled

#endif