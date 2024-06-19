#pragma once
#include "types.h"
#include "wgpu.h"
#include "wgpu_initializers.h"

class RenderableTarget {
 public:
  struct Configuration {
    u32 width;
    u32 height;
    WGPUPresentMode present_mode;
    WGPUDevice device;
    WGPUCompositeAlphaMode alpha_mode = WGPUCompositeAlphaMode_Opaque;
    WGPUTextureFormat format;
    WGPUTextureFormat depth_stencil_format =
        WGPUTextureFormat_Depth24PlusStencil8;
    u32 format_size;
    WGPUTextureUsageFlags usage;
    WGPUTextureFormat* view_formats = nullptr;
    u32 view_format_count = 0;
    u32 multisample_count = 1;
  };

  virtual WGPUTexture get_current_texture() const = 0;
  virtual WGPUTextureFormat get_preferred_format() const = 0;
  virtual u32 get_preferred_format_size() const = 0;
  virtual WGPUTextureView get_current_texture_view() const = 0;
  virtual WGPUTextureFormat get_format() const = 0;
  virtual WGPUSurface get_surface() const = 0;
  virtual void configure(Configuration* configuration) = 0;
  virtual WGPUExtent3D get_extent() const = 0;
  virtual void present() = 0;

  virtual ~RenderableTarget() = default;
};

class OnScreenTarget : public RenderableTarget {
 public:
  OnScreenTarget() = default;
  OnScreenTarget(const OnScreenTarget&) = delete;
  OnScreenTarget& operator=(const OnScreenTarget&) = delete;
  OnScreenTarget& operator=(OnScreenTarget&& other);
  OnScreenTarget(OnScreenTarget&& other);

  ~OnScreenTarget() override;

  WGPUTextureFormat get_preferred_format() const;
  u32 get_preferred_format_size() const;
  WGPUTexture get_current_texture() const override;
  WGPUTextureView get_current_texture_view() const override;
  WGPUTextureFormat get_format() const override;
  WGPUExtent3D get_extent() const override;
  WGPUSurface get_surface() const override;

  void present() override;
  void configure(RenderableTarget::Configuration* configuration) override;

  static OnScreenTarget from_glfw_window(WGPUInstance instance,
                                         struct GLFWwindow* window);
  static OnScreenTarget from_surface(WGPUSurface surface);
  static OnScreenTarget from_html_canvas(WGPUInstance instance,
                                         const char* canvas_selector);

 private:
  WGPUSurface surface_;
  WGPUExtent3D extent_;
  WGPUTextureFormat format_;
  WGPUTexture depth_stencil_texture_ = nullptr;
  WGPUTextureFormat depth_stencil_format_;
};

class OffscreenTarget : public RenderableTarget {
 public:
  OffscreenTarget() = default;
  OffscreenTarget(const OffscreenTarget&) = delete;
  OffscreenTarget& operator=(const OffscreenTarget&) = delete;
  OffscreenTarget& operator=(OffscreenTarget&& other) = delete;
  OffscreenTarget(OffscreenTarget&& other);

  ~OffscreenTarget() override;

  WGPUTextureFormat get_preferred_format() const;
  u32 get_preferred_format_size() const;
  WGPUTexture get_current_texture() const override;
  WGPUTextureView get_current_texture_view() const override;
  WGPUTextureFormat get_format() const override;
  WGPUExtent3D get_extent() const override;
  WGPUSurface get_surface() const override;
  void present() override;
  void configure(RenderableTarget::Configuration* configuration) override;

 private:
  WGPUTexture texture_ = nullptr;
  WGPUBuffer texture_copy_buffer_ = nullptr;
  WGPUExtent3D extent_;
  WGPUTextureFormat format_;
  WGPUTexture depth_stencil_texture_;
  WGPUTextureFormat depth_stencil_format_;
  u32 format_size_;
};
