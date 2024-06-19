#pragma once
#include <tuple>
#include "resource_bind_data.h"
#include "types.h"
#include "wgpu.h"

namespace wgpuinit {
WGPUInstanceDescriptor instance_descriptor();
WGPUDeviceDescriptor device_descriptor();
WGPURequestAdapterOptions request_adapter_options();
WGPUComputePipelineDescriptor compute_pipeline_descriptor(
    WGPUPipelineLayout pipeline_layout, WGPUShaderModule shader_module,
    const char* compute_entry_point);

WGPUShaderModuleDescriptor shader_module_descriptor(const char* code);

WGPUTextureDescriptor texture_descriptor(
    WGPUExtent3D texture_extent, WGPUTextureFormat format,
    WGPUTextureUsageFlags usage,
    WGPUTextureDimension dimension = WGPUTextureDimension_2D,
    u32 mip_level_count = 1, u32 sample_count = 1,
    WGPUTextureFormat* view_formats = nullptr, u32 view_format_count = 0);

WGPUBufferDescriptor buffer_descriptor(u32 size, WGPUBufferUsageFlags usage);

WGPUImageCopyBuffer image_copy_buffer(WGPUBuffer buffer, u32 bytes_per_row,
                                      u32 rows_per_image, u32 offset = 0);

WGPUImageCopyTexture image_copy_texture(
    WGPUTexture texture, u32 mip_level, WGPUOrigin3D origin = {0, 0, 0},
    WGPUTextureAspect aspect = WGPUTextureAspect_All);

std::tuple<WGPUBindGroupEntry, WGPUBindGroupLayoutEntry>
storage_texture_bind_group_entry(WGPUTextureView texture_view,
                                 WGPUTextureFormat texture_format,
                                 WGPUStorageTextureAccess access,
                                 WGPUShaderStageFlags visibility,
                                 WGPUTextureViewDimension view_dimension);

WGPURenderPassDescriptor render_pass_descriptor(
    WGPURenderPassColorAttachment* color_attachments,
    u32 color_attachment_count,
    WGPURenderPassDepthStencilAttachment* depth_stencil_attachment = nullptr);

WGPURenderPassDepthStencilAttachment render_pass_depth_stencil_attachment(
    WGPUTextureView texture_view, f32 depth_load_value = 1,
    WGPULoadOp depth_load_op = WGPULoadOp_Clear,
    WGPUStoreOp depth_store_op = WGPUStoreOp_Store, f32 stencil_load_value = 1,
    WGPULoadOp stencil_load_op = WGPULoadOp_Clear,
    WGPUStoreOp stencil_store_op = WGPUStoreOp_Store);

WGPURenderPassColorAttachment render_pass_color_attachment(
    WGPUTextureView view, WGPUColor clear_value = {0, 0, 0, 1},
    WGPUTextureView resolve_target = nullptr,
    WGPULoadOp load_op = WGPULoadOp_Clear,
    WGPUStoreOp store_op = WGPUStoreOp_Store);

WGPUBlendComponent blend_component(
    WGPUBlendFactor src_factor = WGPUBlendFactor_SrcAlpha,
    WGPUBlendFactor dst_factor = WGPUBlendFactor_OneMinusSrcAlpha,
    WGPUBlendOperation operation = WGPUBlendOperation_Add);

WGPUBlendState blend_state(
    WGPUBlendComponent color_blend_component = blend_component(),
    WGPUBlendComponent alpha_blend_component = blend_component());
WGPUColorTargetState color_target_state(
    WGPUTextureFormat format, WGPUBlendState* blend,
    WGPUColorWriteMask write_mask = WGPUColorWriteMask_All);

WGPUFragmentState fragment_state(WGPUShaderModule module,
                                 WGPUColorTargetState* targets,
                                 u32 target_count,
                                 const char* entry_point = "main");

WGPUMultisampleState multisample_state(u32 count = 1, u32 mask = ~0u,
                                       bool alpha_to_coverage_enabled = false);

WGPUPrimitiveState primitive_state(
    WGPUPrimitiveTopology topology, WGPUCullMode cull_mode = WGPUCullMode_None,
    WGPUIndexFormat strip_index_format = WGPUIndexFormat_Undefined,
    WGPUFrontFace front_face = WGPUFrontFace_CCW);

WGPUStencilFaceState stencil_face_state(
    WGPUCompareFunction compare = WGPUCompareFunction_Always,
    WGPUStencilOperation fail_op = WGPUStencilOperation_Keep,
    WGPUStencilOperation depth_fail_op = WGPUStencilOperation_Keep,
    WGPUStencilOperation pass_op = WGPUStencilOperation_Keep);

WGPUDepthStencilState depth_stencil_state(
    WGPUTextureFormat format = WGPUTextureFormat_Depth24PlusStencil8,
    bool depth_write_enabled = true,
    WGPUStencilFaceState stencil_state = stencil_face_state());

WGPURenderPipelineDescriptor render_pipeline_descriptor(
    WGPUVertexState vertex_state, WGPUMultisampleState multisample_state,
    WGPUPrimitiveState primitive_state, WGPUFragmentState* fragment_state,
    WGPUDepthStencilState* depth_stencil_state,
    WGPUPipelineLayout pipeline_layout);

WGPUPipelineLayoutDescriptor pipeline_layout_descriptor(
    WGPUBindGroupLayout* bind_group_layouts, u32 bind_group_layout_count);

ResourceBindData uniform_resource_bind_data(
    WGPUBuffer buffer, bool has_dynamic_offset = false, u32 binding = 0,
    WGPUShaderStageFlags visibility = WGPUShaderStage_Vertex |
                                      WGPUShaderStage_Fragment,
    u64 offset = 0, u64 size = 0);

ResourceBindData storage_buffer_resource_bind_data(
    WGPUBuffer buffer, bool read_only, bool has_dynamic_offset = false,
    u32 binding = 0,
    WGPUShaderStageFlags visibility = WGPUShaderStage_Fragment |
                                      WGPUShaderStage_Compute,
    u64 offset = 0, u64 size = 0);

WGPUBindGroupLayoutDescriptor bind_group_layout_descriptor(
    WGPUBindGroupLayoutEntry* entries, u32 entry_count);

WGPUBindGroupDescriptor bind_group_descriptor(WGPUBindGroupLayout layout,
                                              WGPUBindGroupEntry* entries,
                                              u32 entry_count);

WGPUVertexState vertex_state(WGPUShaderModule module,
                             WGPUVertexBufferLayout* buffers, u32 buffer_count,
                             const char* entry_point = "main");

WGPUVertexBufferLayout vertex_buffer_layout(
    WGPUVertexAttribute* attributes, u64 attribute_count, u64 array_stride,
    WGPUVertexStepMode step_mode = WGPUVertexStepMode_Vertex);

WGPUVertexAttribute vertex_attribute(u32 shader_location, u64 offset,
                                     WGPUVertexFormat format);
}  // namespace wgpuinit