#ifndef COMPONENTS_UNGOOGLED_PLAYWRIGHT_VIRTUAL_DISPLAY_H_
#define COMPONENTS_UNGOOGLED_PLAYWRIGHT_VIRTUAL_DISPLAY_H_

#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/size.h"

namespace ungoogled {

class VirtualDisplay : public base::RefCountedThreadSafe<VirtualDisplay> {
 public:
  struct DisplayConfig {
    bool render_cursor;
    gfx::Size size;
    struct CursorParams {
      bool use_hardware_cursor = false;
      bool composite_with_content = true;
      int z_order = 1000;
    } cursor;
  };

  VirtualDisplay();
  
  bool Initialize(const DisplayConfig& config);
  void Resize(const gfx::Size& size);
  void CaptureFrame(std::vector<uint8_t>* output);
  void UpdateCursorState(bool visible);
  void RenderCursor(const gfx::Point& position);

 private:
  friend class base::RefCountedThreadSafe<VirtualDisplay>;
  ~VirtualDisplay();

  bool should_render_cursor_ = false;
  std::unique_ptr<CursorRenderer> cursor_renderer_;
  gfx::Size size_;
  std::unique_ptr<uint8_t[]> frame_buffer_;
  
  DISALLOW_COPY_AND_ASSIGN(VirtualDisplay);
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_PLAYWRIGHT_VIRTUAL_DISPLAY_H_
