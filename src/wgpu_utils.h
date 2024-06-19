#pragma once
#include "types.h"
#include "wgpu.h"

namespace wgpuutils {
WGPUInstance create_instance(WGPUInstanceDescriptor* instance_descriptor);

WGPUAdapter request_adapter(WGPUInstance instance,
                            const WGPURequestAdapterOptions* options);

WGPUDevice request_device(WGPUAdapter adapter,
                          const WGPUDeviceDescriptor* device_descriptor);

void copy_texture_to_buffer(WGPUCommandEncoder command_encoder,
                            WGPUTexture texture, WGPUBuffer buffer,
                            u32 format_size, WGPUExtent3D extent);
}  // namespace wgpuutils