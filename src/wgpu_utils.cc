#include "wgpu_utils.h"
#include <cassert>
#include "wgpu_initializers.h"

WGPUInstance wgpuutils::create_instance(WGPUInstanceDescriptor* instance_desc) {
  return wgpuCreateInstance(instance_desc);
}

WGPUDevice wgpuutils::request_device(WGPUAdapter adapter,
                                     const WGPUDeviceDescriptor* device_desc) {
  struct UserData {
    WGPUDevice device;
    bool done = false;
  };
  UserData user_data;

  wgpuAdapterRequestDevice(
      adapter, device_desc,
      [](WGPURequestDeviceStatus status, WGPUDevice device, const char* message,
         void* userdata) {
        UserData* data = reinterpret_cast<UserData*>(userdata);
        data->done = true;
        data->device = device;
      },
      &user_data);

  assert(user_data.done);
  return user_data.device;
}

WGPUAdapter wgpuutils::request_adapter(
    WGPUInstance instance, const WGPURequestAdapterOptions* options) {
  struct UserData {
    WGPUAdapter adapter;
    bool done = false;
  };
  UserData user_data;

  wgpuInstanceRequestAdapter(
      instance, options,
      [](WGPURequestAdapterStatus status, WGPUAdapter adapter, const char*,
         void* userdata) {
        UserData* data = reinterpret_cast<UserData*>(userdata);
        data->done = true;
        data->adapter = adapter;
      },
      &user_data);

  assert(user_data.done);
  return user_data.adapter;
}

void wgpuutils::copy_texture_to_buffer(WGPUCommandEncoder command_encoder,
                                       WGPUTexture texture, WGPUBuffer buffer,
                                       u32 format_size, WGPUExtent3D extent) {
  WGPUImageCopyTexture image_copy_texture =
      wgpuinit::image_copy_texture(texture, 0);

  WGPUImageCopyBuffer image_copy_buffer = wgpuinit::image_copy_buffer(
      buffer, format_size * extent.width, extent.height);

  wgpuCommandEncoderCopyTextureToBuffer(command_encoder, &image_copy_texture,
                                        &image_copy_buffer, &extent);
}