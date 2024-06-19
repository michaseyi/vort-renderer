#include <array>
#include "../constants.h"
#include "../iterator.h"
#include "../renderable_object_data.h"
#include "../wgpu_context.h"
#include "render_pass.h"

Iterator<RenderableObjectData> renderables() {
  for (u32 i = 0; i < 10; i++) {
    co_yield RenderableObjectData{};
  }
}

SceneRenderPass& SceneRenderPass::operator=(SceneRenderPass&& other) {
  cleanup();
  render_texture = other.render_texture;
  depth_stencil_texture = other.depth_stencil_texture;
  multisample_texture = other.multisample_texture;
  multisampled = other.multisampled;
  hdr = other.hdr;
  extent = other.extent;
  depth_stencil_texture_format = other.depth_stencil_texture_format;
  render_texture_format = other.render_texture_format;
  other.render_texture = nullptr;
  other.depth_stencil_texture = nullptr;
  other.multisample_texture = nullptr;
  return *this;
}

SceneRenderPass::SceneRenderPass(SceneRenderPass&& other) {
  render_texture = other.render_texture;
  depth_stencil_texture = other.depth_stencil_texture;
  multisample_texture = other.multisample_texture;
  multisampled = other.multisampled;
  hdr = other.hdr;
  extent = other.extent;
  depth_stencil_texture_format = other.depth_stencil_texture_format;
  render_texture_format = other.render_texture_format;
  other.render_texture = nullptr;
  other.depth_stencil_texture = nullptr;
  other.multisample_texture = nullptr;
}

SceneRenderPass::~SceneRenderPass() {
  cleanup();
}

void SceneRenderPass::cleanup() {
  destroy_texture(render_texture);
  destroy_texture(depth_stencil_texture);
  destroy_texture(multisample_texture);
}

void SceneRenderPass::destroy_texture(WGPUTexture texture) {
  if (texture) {
    wgpuTextureRelease(texture);
    wgpuTextureDestroy(texture);
  }
}

void SceneRenderPass::configure(ResourceTable& resource_table) {
  auto output_texture_entry =
      resource_table.get(Constants::output_texture).as_texture();

  u32 width = output_texture_entry.size.width,
      height = output_texture_entry.size.height;

  if (width == extent.width && height == extent.height) {
    return;
  }

  cleanup();
  
  depth_stencil_texture_format = WGPUTextureFormat_Depth24PlusStencil8;
  render_texture_format =
      hdr ? WGPUTextureFormat_RGBA16Float : WGPUTextureFormat_RGBA8Unorm;
  auto& context = WGPUContext::get();

  auto render_texture_desc = wgpuinit::texture_descriptor(
      {width, height, 1}, render_texture_format,
      WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_StorageBinding);

  render_texture =
      wgpuDeviceCreateTexture(context.device, &render_texture_desc);
  assert(render_texture);

  auto depth_stencil_texture_desc = wgpuinit::texture_descriptor(
      {width, height, 1}, depth_stencil_texture_format,
      WGPUTextureUsage_RenderAttachment, WGPUTextureDimension_2D, 1,
      multisampled ? Constants::multisampling_level : 1);

  depth_stencil_texture =
      wgpuDeviceCreateTexture(context.device, &depth_stencil_texture_desc);
  assert(depth_stencil_texture);

  if (multisampled) {
    auto multisample_texture_desc = wgpuinit::texture_descriptor(
        {width, height, 1}, render_texture_format,
        WGPUTextureUsage_RenderAttachment, WGPUTextureDimension_2D, 1,
        multisampled ? Constants::multisampling_level : 1);
    multisample_texture =
        wgpuDeviceCreateTexture(context.device, &multisample_texture_desc);
    assert(multisample_texture);
  }
  extent = {width, height, 1};

  resource_table.set(Constants::scene_color_texture,
                     ResourceTable::Entry::from_texture(render_texture));
}

void SceneRenderPass::execute(WGPUCommandEncoder command_encoder,
                              ecs::Command command,
                              ResourceTable& resource_table) const {
  (void)resource_table;

  auto& context = WGPUContext::get();

  auto render_texture_view = wgpuTextureCreateView(render_texture, nullptr);
  auto depth_stencil_texture_view =
      wgpuTextureCreateView(depth_stencil_texture, nullptr);
  auto multisample_texture_view =
      multisampled ? wgpuTextureCreateView(multisample_texture, nullptr)
                   : nullptr;
  WGPUColor clear_value = {0, 0, 0, 1};

  std::array<WGPURenderPassColorAttachment, 1> render_pass_color_attachments = {
      multisampled
          ? wgpuinit::render_pass_color_attachment(
                multisample_texture_view, clear_value, render_texture_view)
          : wgpuinit::render_pass_color_attachment(render_texture_view,
                                                   clear_value)};

  WGPURenderPassDepthStencilAttachment render_pass_depth_stencil_attachment =
      wgpuinit::render_pass_depth_stencil_attachment(
          depth_stencil_texture_view);

  WGPURenderPassDescriptor render_pass_desc =
      wgpuinit::render_pass_descriptor(render_pass_color_attachments.data(),
                                       render_pass_color_attachments.size(),
                                       &render_pass_depth_stencil_attachment);

  WGPURenderPassEncoder render_pass_encoder =
      wgpuCommandEncoderBeginRenderPass(command_encoder, &render_pass_desc);

  for (RenderableObjectData& renderable : renderables()) {

#pragma region vertex state settings
    WGPUVertexState vertex_state = wgpuinit::vertex_state(
        renderable.vertex_shader, renderable.vertex_buffer_layouts.data(),
        renderable.vertex_buffer_count, renderable.vertex_shader_entry_point);
#pragma endregion

#pragma region multisampling state settings
    WGPUMultisampleState multisample_state = wgpuinit::multisample_state(
        multisampled ? Constants::multisampling_level : 1);
#pragma endregion

#pragma region primitive state settings
    WGPUPrimitiveState primitive_state =
        wgpuinit::primitive_state(renderable.primitive_topology,
                                  WGPUCullMode_None, renderable.index_format);
#pragma endregion

#pragma region depth stencil state settings
    WGPUDepthStencilState depth_stencil_state =
        wgpuinit::depth_stencil_state(depth_stencil_texture_format);
#pragma endregion

#pragma region fragment state settings
    WGPUBlendState blend_state = wgpuinit::blend_state();
    std::array<WGPUColorTargetState, 1> color_target_states = {
        wgpuinit::color_target_state(render_texture_format, &blend_state),
    };
    WGPUFragmentState fragment_state = wgpuinit::fragment_state(
        renderable.fragment_shader, color_target_states.data(),
        color_target_states.size(), renderable.fragment_shader_entry_point);
#pragma endregion

    WGPURenderPipelineDescriptor render_pipeline_desc =
        wgpuinit::render_pipeline_descriptor(vertex_state, multisample_state,
                                             primitive_state, &fragment_state,
                                             &depth_stencil_state, nullptr);

    WGPURenderPipeline render_pipeline =
        wgpuDeviceCreateRenderPipeline(context.device, &render_pipeline_desc);

    wgpuRenderPassEncoderSetPipeline(render_pass_encoder, render_pipeline);

    for (u32 i = 0; i < renderable.vertex_buffer_count; i++) {
      wgpuRenderPassEncoderSetVertexBuffer(
          render_pass_encoder, i, renderable.vertex_buffers[i],
          renderable.vertex_buffer_ranges[i].offset,
          renderable.vertex_buffer_ranges[i].size);
    }

    wgpuRenderPassEncoderSetIndexBuffer(
        render_pass_encoder, renderable.index_buffer, renderable.index_format,
        renderable.index_buffer_range.offset,
        renderable.index_buffer_range.size);

    wgpuRenderPassEncoderDrawIndexed(
        render_pass_encoder, renderable.vertex_shader_invoke_count, 1, 0, 0, 0);
  }

  wgpuRenderPassEncoderEnd(render_pass_encoder);
  wgpuRenderPassEncoderRelease(render_pass_encoder);

  wgpuTextureViewRelease(render_texture_view);
  wgpuTextureViewRelease(depth_stencil_texture_view);

  if (multisampled) {
    wgpuTextureViewRelease(multisample_texture_view);
  }
}