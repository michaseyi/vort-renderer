#pragma once
#include "types.h"
#include "wgpu.h"

namespace wgpuutils {
void copy_texture_to_buffer(WGPUCommandEncoder encoder, WGPUTexture texture,
                            WGPUBuffer buffer, WGPUExtent3D copy_size,
                            u32 channel_count);
}