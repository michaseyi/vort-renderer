#include <array>
#include "../constants.h"
#include "render_pass.h"

void DummyRenderPass::configure(ResourceTable& resource_table) {
  (void)resource_table;
};

void DummyRenderPass::execute(WGPUCommandEncoder command_encoder,
                              ecs::Command command,
                              ResourceTable& resource_table) const {
  (void)command;
  auto output_texture =
      resource_table.get(Constants::output_texture).as_texture();

  auto output_texture_view =
      wgpuTextureCreateView(output_texture.texture, nullptr);

  std::array<WGPURenderPassColorAttachment, 4> color_attachments = {
      wgpuinit::render_pass_color_attachment(output_texture_view,
                                             {1, 0, 0, 1})};

  auto render_pass_descriptor = wgpuinit::render_pass_descriptor(
      color_attachments.data(), color_attachments.size(), nullptr);

  auto render_pass_encoder = wgpuCommandEncoderBeginRenderPass(
      command_encoder, &render_pass_descriptor);

  wgpuRenderPassEncoderEnd(render_pass_encoder);

  wgpuRenderPassEncoderRelease(render_pass_encoder);
  wgpuTextureViewRelease(output_texture_view);
}