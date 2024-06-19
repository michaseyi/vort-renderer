#include <cassert>
#include "renderable_target.h"
#include "wgpu_context.h"
#include "wgpu_utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

WGPUTexture OffscreenTarget::get_current_texture() const {
  return texture_;
}
WGPUTextureView OffscreenTarget::get_current_texture_view() const {
  return wgpuTextureCreateView(texture_, nullptr);
}
WGPUTextureFormat OffscreenTarget::get_format() const {
  return format_;
}
WGPUExtent3D OffscreenTarget::get_extent() const {
  return extent_;
}

WGPUSurface OffscreenTarget::get_surface() const {
  return nullptr;
}

void OffscreenTarget::configure(
    RenderableTarget::Configuration* configuration) {
  extent_ = {configuration->width, configuration->height, 1};
  format_ = configuration->format;
  format_size_ = configuration->format_size;

  WGPUTextureDescriptor texture_descriptor = wgpuinit::texture_descriptor(
      extent_, format_,
      configuration->usage | WGPUTextureUsage_RenderAttachment |
          WGPUTextureUsage_CopySrc | WGPUTextureUsage_StorageBinding,
      WGPUTextureDimension_2D, 1, 1, configuration->view_formats,
      configuration->view_format_count);

  if (texture_) {
    wgpuTextureDestroy(texture_);
    wgpuTextureRelease(texture_);
  }

  texture_ =
      wgpuDeviceCreateTexture(configuration->device, &texture_descriptor);
  assert(texture_ != nullptr);

  WGPUBufferDescriptor buffer_descriptor = wgpuinit::buffer_descriptor(
      configuration->width * configuration->height * configuration->format_size,
      WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead);

  if (texture_copy_buffer_) {
    wgpuBufferDestroy(texture_copy_buffer_);
    wgpuBufferRelease(texture_copy_buffer_);
  }
  texture_copy_buffer_ =
      wgpuDeviceCreateBuffer(configuration->device, &buffer_descriptor);

  assert(texture_copy_buffer_ != nullptr);
}

OffscreenTarget::~OffscreenTarget() {
  if (texture_) {
    wgpuTextureDestroy(texture_);
    wgpuTextureRelease(texture_);
  }

  if (texture_copy_buffer_) {
    wgpuBufferDestroy(texture_copy_buffer_);
    wgpuBufferRelease(texture_copy_buffer_);
  }
}

OffscreenTarget::OffscreenTarget(OffscreenTarget&& other) {
  texture_ = other.texture_;
  texture_copy_buffer_ = other.texture_copy_buffer_;
  extent_ = other.extent_;
  format_ = other.format_;
  format_size_ = other.format_size_;

  other.texture_ = nullptr;
  other.texture_copy_buffer_ = nullptr;
}

WGPUTextureFormat OffscreenTarget::get_preferred_format() const {
  return WGPUTextureFormat_RGBA8Unorm;
};
u32 OffscreenTarget::get_preferred_format_size() const {
  return 4;
};

void OffscreenTarget::present() {
  auto& context = WGPUContext::get();

  WGPUCommandEncoderDescriptor command_encoder_desc = {};
  WGPUCommandEncoder command_encoder =
      wgpuDeviceCreateCommandEncoder(context.device, &command_encoder_desc);

  wgpuutils::copy_texture_to_buffer(
      command_encoder, texture_, texture_copy_buffer_, format_size_, extent_);

  WGPUCommandBufferDescriptor command_buffer_desc = {};
  WGPUCommandBuffer command_buffer =
      wgpuCommandEncoderFinish(command_encoder, &command_buffer_desc);

  wgpuQueueSubmit(context.queue, 1, &command_buffer);

  wgpuCommandBufferRelease(command_buffer);
  wgpuCommandEncoderRelease(command_encoder);

  struct UserData {
    WGPUBuffer buffer;
    WGPUExtent3D extent;
    u32 format_size;
  };

  UserData user_data{texture_copy_buffer_, extent_, format_size_};

  wgpuBufferMapAsync(
      texture_copy_buffer_, WGPUMapMode_Read, 0,
      format_size_ * extent_.width * extent_.height,
      [](WGPUBufferMapAsyncStatus status, void* userdata_) {
        UserData& user_data = *reinterpret_cast<UserData*>(userdata_);
        if (status == WGPUBufferMapAsyncStatus_Success) {
          const void* data = wgpuBufferGetConstMappedRange(
              user_data.buffer, 0,
              user_data.format_size * user_data.extent.width *
                  user_data.extent.height);
          stbi_write_png("output.png", user_data.extent.width,
                         user_data.extent.height, user_data.format_size, data,
                         user_data.extent.width * user_data.format_size);
          wgpuBufferUnmap(user_data.buffer);
        }
      },
      &user_data);

  wgpuDevicePoll(context.device, true, nullptr);
}