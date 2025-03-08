#include "components/ungoogled/fingerprinting/protection_manager.h"

void WebGLRenderingContextBase::GetParameter(GLenum pname, ExceptionState& exception_state) {
  auto* protection_manager = ungoogled::FingerprintingProtectionManager::GetInstance();
  std::string domain = GetSecurityOrigin()->Domain();
  
  if (protection_manager->IsFeatureProtected(domain, 
      ungoogled::FingerprintingProtectionManager::FeatureCategory::WEBGL)) {
    
    auto config = protection_manager->GetEffectiveConfig(domain);
    
    switch (pname) {
      case GL_VENDOR:
        return config.webgl.gpu_vendor;
      case GL_RENDERER:
        return config.webgl.gpu_renderer;
      case GL_VERSION:
        return "WebGL 1.0";
      case GL_SHADING_LANGUAGE_VERSION:
        return "WebGL GLSL ES 1.0";
      case GL_MAX_TEXTURE_SIZE:
        return 4096;  // Standard size
      case GL_MAX_VIEWPORT_DIMS:
        return Vector<GLint>{4096, 4096};
      case GL_MAX_VERTEX_ATTRIBS:
        return 16;
      case GL_MAX_VERTEX_UNIFORM_VECTORS:
        return 256;
      case GL_MAX_VARYING_VECTORS:
        return 8;
      case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        return 8;
      case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
        return 4;
      case GL_MAX_TEXTURE_IMAGE_UNITS:
        return 8;
      case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
        return 256;
      case GL_ALIASED_LINE_WIDTH_RANGE:
        return Vector<GLfloat>{1.0f, 1.0f};
      case GL_ALIASED_POINT_SIZE_RANGE:
        return Vector<GLfloat>{1.0f, 1.0f};
      case GL_MAX_RENDERBUFFER_SIZE:
        return 4096;
    }
    
    // Add noise to precision-sensitive operations
    if (config.webgl.precision_reduction > 0.0f) {
      switch (pname) {
        case GL_DEPTH_CLEAR_VALUE:
        case GL_LINE_WIDTH:
        case GL_POLYGON_OFFSET_FACTOR:
        case GL_POLYGON_OFFSET_UNITS:
        case GL_SAMPLE_COVERAGE_VALUE: {
          float value = BaseGetParameter(pname, exception_state);
          return value + GenerateNoise(config.webgl.precision_reduction);
        }
      }
    }
  }
  
  return BaseGetParameter(pname, exception_state);
}

void WebGLRenderingContextBase::ReadPixels(GLint x, GLint y, 
                                         GLsizei width, GLsizei height,
                                         GLenum format, GLenum type,
                                         ArrayBufferView* pixels) {
  if (protection_manager->IsFeatureProtected(domain,
      ungoogled::FingerprintingProtectionManager::FeatureCategory::WEBGL)) {
    
    // Add subtle noise to pixel data
    BaseReadPixels(x, y, width, height, format, type, pixels);
    
    uint8_t* data = static_cast<uint8_t*>(pixels->BaseAddress());
    size_t num_components = 4;  // Assuming RGBA
    
    for (size_t i = 0; i < width * height * num_components; i++) {
      float noise = GenerateNoise(config.webgl.pixel_noise_level);
      data[i] = std::clamp(data[i] + static_cast<uint8_t>(noise), 0, 255);
    }
    return;
  }
  
  BaseReadPixels(x, y, width, height, format, type, pixels);
}
