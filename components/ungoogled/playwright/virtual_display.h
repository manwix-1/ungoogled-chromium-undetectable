#ifndef COMPONENTS_UNGOOGLED_PLAYWRIGHT_VIRTUAL_DISPLAY_H_
#define COMPONENTS_UNGOOGLED_PLAYWRIGHT_VIRTUAL_DISPLAY_H_

#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/size.h"

namespace ungoogled {

class VirtualDisplay : public base::RefCountedThreadSafe<VirtualDisplay> {
 public:
  VirtualDisplay();
  
  bool Initialize(const gfx::Size& size);
  void Resize(const gfx::Size& size);
  void CaptureFrame(std::vector<uint8_t>* output);
  
 private:
  friend class base::RefCountedThreadSafe<VirtualDisplay>;
  ~VirtualDisplay();

  gfx::Size size_;
  std::unique_ptr<uint8_t[]> frame_buffer_;
  
  DISALLOW_COPY_AND_ASSIGN(VirtualDisplay);
};

}  // namespace ungoogled

#endif  // COMPONENTS_UNGOOGLED_PLAYWRIGHT_VIRTUAL_DISPLAY_H_