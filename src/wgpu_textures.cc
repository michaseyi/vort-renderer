#include "wgpu_textures.h"
#include "wgpu_initializers.h"

void wgpuutils::copy_texture_to_buffer(WGPUCommandEncoder encoder,
                                       WGPUTexture texture, WGPUBuffer buffer,
                                       WGPUExtent3D copy_size,
                                       u32 channel_count) {
  WGPUImageCopyTexture image_copy_texture =
      wgpuinit::image_copy_texture(texture, 0);

  WGPUImageCopyBuffer image_copy_buffer = wgpuinit::image_copy_buffer(
      buffer, copy_size.width * channel_count, copy_size.height);

  wgpuCommandEncoderCopyTextureToBuffer(encoder, &image_copy_texture,
                                        &image_copy_buffer, &copy_size);
}