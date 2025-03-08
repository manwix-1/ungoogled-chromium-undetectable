#include "components/ungoogled/fingerprinting/protections/clipboard_protection.h"

namespace ungoogled {

bool ClipboardProtection::HandleClipboardOperation(
    ClipboardOp op, const std::string& origin) {
  if (!config_.protect_clipboard) {
    return true;
  }

  switch (config_.clipboard_policy) {
    case ClipboardPolicy::BLOCK_ALL:
      return false;
    case ClipboardPolicy::BLOCK_WRITE:
      return op == ClipboardOp::READ;
    case ClipboardPolicy::BLOCK_READ:
      return op == ClipboardOp::WRITE;
    case ClipboardPolicy::PROMPT:
      return ShowClipboardPrompt(op, origin);
    case ClipboardPolicy::SANITIZE:
      return HandleSanitizedOperation(op);
  }
  return false;
}

bool ClipboardProtection::HandleSanitizedOperation(ClipboardOp op) {
  switch (op) {
    case ClipboardOp::READ:
      return SanitizeClipboardData();
    case ClipboardOp::WRITE:
      return SanitizeWriteData();
    case ClipboardOp::READ_HTML:
      return StripHTMLFormatting();
    case ClipboardOp::WRITE_IMAGE:
      return SanitizeImageData();
  }
  return false;
}

void ClipboardProtection::SanitizeClipboardData() {
  if (!config_.sanitize_clipboard) {
    return;
  }

  // Remove metadata
  RemoveMetadata();
  
  // Strip formatting if configured
  if (config_.strip_formatting) {
    StripFormatting();
  }

  // Remove tracking content
  if (config_.remove_tracking) {
    RemoveTrackingContent();
  }
}

} // namespace ungoogled