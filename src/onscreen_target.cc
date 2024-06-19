
#include <cassert>
#include "glfw3webgpu.h"
#include "renderable_target.h"

OnScreenTarget::OnScreenTarget(OnScreenTarget&& other) {
  surface_ = other.surface_;
  extent_ = other.extent_;
  format_ = other.format_;

  other.surface_ = nullptr;
}

OnScreenTarget& OnScreenTarget::operator=(OnScreenTarget&& other) {
  surface_ = other.surface_;
  extent_ = other.extent_;
  format_ = other.format_;

  other.surface_ = nullptr;
  return *this;
}

OnScreenTarget::~OnScreenTarget() {
  if (surface_) {
    wgpuSurfaceRelease(surface_);
  }
}

// Do not destroy the returned texture, the ownership is handled by the renderable target
WGPUTexture OnScreenTarget::get_current_texture() const {
  assert(surface_ != nullptr);
  WGPUSurfaceTexture surface_texture;
  wgpuSurfaceGetCurrentTexture(surface_, &surface_texture);
  assert(surface_texture.status == WGPUSurfaceGetCurrentTextureStatus_Success);
  assert(surface_texture.texture != nullptr);
  assert(surface_texture.suboptimal == false);
  return surface_texture.texture;
}

WGPUTextureView OnScreenTarget::get_current_texture_view() const {
  return wgpuTextureCreateView(get_current_texture(), nullptr);
}

WGPUTextureFormat OnScreenTarget::get_format() const {
  return format_;
}

WGPUExtent3D OnScreenTarget::get_extent() const {
  return extent_;
}

WGPUSurface OnScreenTarget::get_surface() const {
  assert(surface_ != nullptr);
  return surface_;
}

void OnScreenTarget::present() {
  assert(surface_ != nullptr);
  wgpuSurfacePresent(surface_);
}

void OnScreenTarget::configure(RenderableTarget::Configuration* configuration) {
  assert(surface_ != nullptr);
  format_ = configuration->format;
  extent_ = {configuration->width, configuration->height, 1};

  WGPUSurfaceConfiguration surface_configuration = {};
  surface_configuration.usage =
      WGPUTextureUsage_RenderAttachment | configuration->usage;
  surface_configuration.format = format_;
  surface_configuration.width = extent_.width;
  surface_configuration.height = extent_.height;
  surface_configuration.presentMode = configuration->present_mode;
  surface_configuration.alphaMode = configuration->alpha_mode;
  surface_configuration.device = configuration->device;
  surface_configuration.viewFormatCount = configuration->view_format_count;
  surface_configuration.viewFormats = configuration->view_formats;

  wgpuSurfaceConfigure(surface_, &surface_configuration);
}

OnScreenTarget OnScreenTarget::from_glfw_window(WGPUInstance instance,
                                                GLFWwindow* window) {
  OnScreenTarget target;
  target.surface_ = glfwGetWGPUSurface(instance, window);
  return target;
}

OnScreenTarget OnScreenTarget::from_surface(WGPUSurface surface) {
  OnScreenTarget target;
  target.surface_ = surface;
  return target;
}
WGPUTextureFormat OnScreenTarget::get_preferred_format() const {
  return WGPUTextureFormat_BGRA8Unorm;  // or wgpuSurfaceGetPreferredFormat(surface)
};
u32 OnScreenTarget::get_preferred_format_size() const {
  return 4;
};

OnScreenTarget OnScreenTarget::from_html_canvas(WGPUInstance instance,
                                                const char* canvas_selector) {
  OnScreenTarget target;
  WGPUSurfaceDescriptorFromCanvasHTMLSelector canvas = {};
  canvas.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
  canvas.selector = canvas_selector;

  WGPUSurfaceDescriptor surface_descriptor = {};
  surface_descriptor.nextInChain =
      reinterpret_cast<const WGPUChainedStruct*>(&canvas);

  target.surface_ = wgpuInstanceCreateSurface(instance, &surface_descriptor);
  return target;
}