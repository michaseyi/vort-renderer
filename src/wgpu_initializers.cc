#include "wgpu_initializers.h"

WGPUInstanceDescriptor wgpuinit::instance_descriptor() {
  WGPUInstanceDescriptor desc = {};
  return desc;
}
WGPUDeviceDescriptor wgpuinit::device_descriptor() {
  WGPUDeviceDescriptor desc = {};
  return desc;
}
WGPURequestAdapterOptions wgpuinit::request_adapter_options() {
  WGPURequestAdapterOptions options = {};
  return options;
}

WGPUComputePipelineDescriptor wgpuinit::compute_pipeline_descriptor(
    WGPUPipelineLayout pipeline_layout, WGPUShaderModule shader_module,
    const char* compute_entry_point) {
  WGPUComputePipelineDescriptor desc = {};
  desc.compute.module = shader_module;
  desc.compute.entryPoint = compute_entry_point;
  desc.layout = pipeline_layout;
  return desc;
}

WGPUPipelineLayoutDescriptor wgpuinit::pipeline_layout_descriptor(
    WGPUBindGroupLayout* bind_group_layouts, u32 bind_group_layout_count) {
  WGPUPipelineLayoutDescriptor desc = {};
  desc.bindGroupLayouts = bind_group_layouts;
  desc.bindGroupLayoutCount = bind_group_layout_count;
  return desc;
}

WGPUShaderModuleDescriptor wgpuinit::shader_module_descriptor(
    const char* code) {
  WGPUShaderModuleDescriptor desc = {};

  WGPUShaderModuleWGSLDescriptor wgsl_desc = {};
  wgsl_desc.code = code;
  wgsl_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;

  desc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&wgsl_desc);
  return desc;
}

std::tuple<WGPUBindGroupEntry, WGPUBindGroupLayoutEntry>
wgpuinit::storage_texture_bind_group_entry(
    WGPUTextureView texture_view, WGPUTextureFormat texture_format,
    WGPUStorageTextureAccess access, WGPUShaderStageFlags visibility,
    WGPUTextureViewDimension view_dimension) {

  WGPUBindGroupEntry bind_group_entry = {};
  bind_group_entry.textureView = texture_view;

  WGPUBindGroupLayoutEntry bind_group_layout_entry = {};
  bind_group_layout_entry.storageTexture.format = texture_format;
  bind_group_layout_entry.storageTexture.access = access;
  bind_group_layout_entry.storageTexture.viewDimension = view_dimension;
  bind_group_layout_entry.visibility = visibility;

  return {bind_group_entry, bind_group_layout_entry};
}

WGPUBufferDescriptor wgpuinit::buffer_descriptor(u32 size,
                                                 WGPUBufferUsageFlags usage) {
  WGPUBufferDescriptor desc = {};
  desc.size = size;
  desc.usage = usage;
  return desc;
}

WGPUTextureDescriptor wgpuinit::texture_descriptor(
    WGPUExtent3D texture_extent, WGPUTextureFormat format,
    WGPUTextureUsageFlags usage, WGPUTextureDimension dimension,
    u32 mip_level_count, u32 sample_count, WGPUTextureFormat* view_formats,
    u32 view_format_count) {
  WGPUTextureDescriptor desc = {};
  desc.size = texture_extent;
  desc.format = format;
  desc.usage = usage;
  desc.dimension = dimension;
  desc.mipLevelCount = mip_level_count;
  desc.sampleCount = sample_count;
  desc.viewFormatCount = view_format_count;
  desc.viewFormats = view_formats;
  return desc;
}

WGPUImageCopyBuffer wgpuinit::image_copy_buffer(WGPUBuffer buffer,
                                                u32 bytes_per_row,
                                                u32 rows_per_image,
                                                u32 offset) {
  WGPUImageCopyBuffer desc = {};
  desc.buffer = buffer;
  desc.layout.bytesPerRow = bytes_per_row;
  desc.layout.rowsPerImage = rows_per_image;
  desc.layout.offset = offset;
  return desc;
}

WGPUImageCopyTexture wgpuinit::image_copy_texture(WGPUTexture texture,
                                                  u32 mip_level,
                                                  WGPUOrigin3D origin,
                                                  WGPUTextureAspect aspect) {
  WGPUImageCopyTexture desc = {};
  desc.texture = texture;
  desc.mipLevel = mip_level;
  desc.origin = origin;
  desc.aspect = aspect;
  return desc;
}

WGPURenderPassDescriptor wgpuinit::render_pass_descriptor(
    WGPURenderPassColorAttachment* color_attachments,
    u32 color_attachment_count,
    WGPURenderPassDepthStencilAttachment* depth_stencil_attachment) {
  WGPURenderPassDescriptor render_pass_desc = {};
  render_pass_desc.colorAttachmentCount = color_attachment_count;
  render_pass_desc.colorAttachments = color_attachments;
  render_pass_desc.depthStencilAttachment = depth_stencil_attachment;
  return render_pass_desc;
}

WGPURenderPassDepthStencilAttachment
wgpuinit::render_pass_depth_stencil_attachment(WGPUTextureView texture_view,
                                               f32 depth_load_value,
                                               WGPULoadOp depth_load_op,
                                               WGPUStoreOp depth_store_op,
                                               f32 stencil_load_value,
                                               WGPULoadOp stencil_load_op,
                                               WGPUStoreOp stencil_store_op

) {
  WGPURenderPassDepthStencilAttachment desc = {};
  desc.view = texture_view;
  desc.depthClearValue = depth_load_value;
  desc.depthLoadOp = depth_load_op;
  desc.depthStoreOp = depth_store_op;
  desc.depthReadOnly = false;
  desc.stencilClearValue = stencil_load_value;
  desc.stencilReadOnly = false;
  desc.stencilLoadOp = stencil_load_op;
  desc.stencilStoreOp = stencil_store_op;
  return desc;
}

WGPURenderPassColorAttachment wgpuinit::render_pass_color_attachment(
    WGPUTextureView view, WGPUColor clear_value, WGPUTextureView resolve_target,
    WGPULoadOp load_op, WGPUStoreOp store_op

) {
  WGPURenderPassColorAttachment desc = {};
  desc.clearValue = clear_value;
  desc.loadOp = load_op;
  desc.storeOp = store_op;
  desc.view = view;
  desc.resolveTarget = resolve_target;
  return desc;
}

WGPUBlendComponent wgpuinit::blend_component(WGPUBlendFactor src_factor,
                                             WGPUBlendFactor dst_factor,
                                             WGPUBlendOperation operation) {
  WGPUBlendComponent component = {};
  component.srcFactor = src_factor;
  component.dstFactor = dst_factor;
  component.operation = operation;
  return component;
}

WGPUBlendState wgpuinit::blend_state(WGPUBlendComponent color_blend_component,
                                     WGPUBlendComponent alpha_blend_component) {
  WGPUBlendState state = {};
  state.alpha = alpha_blend_component;
  state.color = color_blend_component;
  return state;
}
WGPUColorTargetState wgpuinit::color_target_state(
    WGPUTextureFormat format, WGPUBlendState* blend,
    WGPUColorWriteMask write_mask) {
  WGPUColorTargetState state = {};
  state.format = format;
  state.blend = blend;
  state.writeMask = write_mask;
  return state;
}

WGPUFragmentState wgpuinit::fragment_state(WGPUShaderModule module,
                                           WGPUColorTargetState* targets,
                                           u32 target_count,
                                           const char* entry_point) {
  WGPUFragmentState state = {};
  state.module = module;
  state.entryPoint = entry_point;
  state.targetCount = target_count;
  state.targets = targets;
  return state;
}

WGPUMultisampleState wgpuinit::multisample_state(
    u32 count, u32 mask, bool alpha_to_coverage_enabled) {
  WGPUMultisampleState state = {};
  state.count = count;
  state.mask = mask;
  state.alphaToCoverageEnabled = alpha_to_coverage_enabled;
  return state;
}

WGPUPrimitiveState wgpuinit::primitive_state(WGPUPrimitiveTopology topology,
                                             WGPUCullMode cull_mode,
                                             WGPUIndexFormat strip_index_format,
                                             WGPUFrontFace front_face) {
  WGPUPrimitiveState state = {};
  state.topology = topology;
  state.stripIndexFormat = strip_index_format;
  state.frontFace = front_face;
  state.cullMode = cull_mode;

  return state;
}
WGPUStencilFaceState wgpuinit::stencil_face_state(
    WGPUCompareFunction compare, WGPUStencilOperation fail_op,
    WGPUStencilOperation depth_fail_op, WGPUStencilOperation pass_op) {
  WGPUStencilFaceState state = {};
  state.compare = compare;
  state.failOp = fail_op;
  state.depthFailOp = depth_fail_op;
  state.passOp = pass_op;
  return state;
}

WGPUDepthStencilState wgpuinit::depth_stencil_state(
    WGPUTextureFormat format, bool depth_write_enabled,
    WGPUStencilFaceState stencil_state) {
  WGPUDepthStencilState state = {};
  state.format = format;
  state.depthWriteEnabled = depth_write_enabled;
  state.depthCompare = WGPUCompareFunction_LessEqual;
  state.depthBias = 0;
  state.depthBiasSlopeScale = 0.0f;
  state.depthBiasClamp = 0.0f;
  state.stencilFront = stencil_state;
  state.stencilBack = stencil_state;
  state.stencilReadMask = ~0u;
  state.stencilWriteMask = ~0u;
  return state;
}

WGPURenderPipelineDescriptor wgpuinit::render_pipeline_descriptor(
    WGPUVertexState vertex_state, WGPUMultisampleState multisample_state,
    WGPUPrimitiveState primitive_state, WGPUFragmentState* fragment_state,
    WGPUDepthStencilState* depth_stencil_state,
    WGPUPipelineLayout pipeline_layout) {
  WGPURenderPipelineDescriptor desc = {};
  desc.layout = pipeline_layout;
  desc.vertex = vertex_state;
  desc.fragment = fragment_state;
  desc.depthStencil = depth_stencil_state;
  desc.multisample = multisample_state;
  desc.primitive = primitive_state;
  return desc;
}

ResourceBindData wgpuinit::uniform_resource_bind_data(
    WGPUBuffer buffer, bool has_dynamic_offset, u32 binding,
    WGPUShaderStageFlags visibility, u64 offset, u64 size) {
  WGPUBindGroupLayoutEntry bind_group_layout_entry = {};
  bind_group_layout_entry.binding = binding;
  bind_group_layout_entry.visibility = visibility;
  bind_group_layout_entry.buffer.type = WGPUBufferBindingType_Uniform;
  bind_group_layout_entry.buffer.minBindingSize = 0;
  bind_group_layout_entry.buffer.hasDynamicOffset = has_dynamic_offset;

  WGPUBindGroupEntry bind_group_entry = {};
  bind_group_entry.binding = binding;
  bind_group_entry.buffer = buffer;
  bind_group_entry.offset = offset;
  bind_group_entry.size = size;

  return {bind_group_entry, bind_group_layout_entry};
}

ResourceBindData wgpuinit::storage_buffer_resource_bind_data(
    WGPUBuffer buffer, bool read_only, bool has_dynamic_offset, u32 binding,
    WGPUShaderStageFlags visibility, u64 offset, u64 size) {
  WGPUBindGroupLayoutEntry bind_group_layout_entry = {};
  bind_group_layout_entry.binding = binding;
  bind_group_layout_entry.visibility = visibility;
  bind_group_layout_entry.buffer.type =
      read_only ? WGPUBufferBindingType_ReadOnlyStorage
                : WGPUBufferBindingType_Storage;
  bind_group_layout_entry.buffer.minBindingSize = 0;
  bind_group_layout_entry.buffer.hasDynamicOffset = has_dynamic_offset;

  WGPUBindGroupEntry bind_group_entry = {};
  bind_group_entry.binding = binding;
  bind_group_entry.buffer = buffer;
  bind_group_entry.offset = offset;
  bind_group_entry.size = size;

  return {bind_group_entry, bind_group_layout_entry};
}

WGPUBindGroupLayoutDescriptor wgpuinit::bind_group_layout_descriptor(
    WGPUBindGroupLayoutEntry* entries, u32 entry_count) {
  WGPUBindGroupLayoutDescriptor desc = {};
  desc.entries = entries;
  desc.entryCount = entry_count;
  return desc;
}

WGPUBindGroupDescriptor wgpuinit::bind_group_descriptor(
    WGPUBindGroupLayout layout, WGPUBindGroupEntry* entries, u32 entry_count) {
  WGPUBindGroupDescriptor desc = {};
  desc.layout = layout;
  desc.entryCount = entry_count;
  desc.entries = entries;
  return desc;
}

WGPUVertexState wgpuinit::vertex_state(WGPUShaderModule module,
                                       WGPUVertexBufferLayout* buffers,
                                       u32 buffer_count,
                                       const char* entry_point) {
  WGPUVertexState state = {};
  state.entryPoint = entry_point;
  state.module = module;
  state.bufferCount = buffer_count;
  state.buffers = buffers;
  return state;
}

WGPUVertexBufferLayout wgpuinit::vertex_buffer_layout(
    WGPUVertexAttribute* attributes, u64 attribute_count, u64 array_stride,
    WGPUVertexStepMode step_mode) {
  WGPUVertexBufferLayout layout = {};
  layout.arrayStride = array_stride;
  layout.attributeCount = attribute_count;
  layout.attributes = attributes;
  layout.stepMode = step_mode;
  return layout;
}

WGPUVertexAttribute wgpuinit::vertex_attribute(u32 shader_location, u64 offset,
                                               WGPUVertexFormat format) {
  WGPUVertexAttribute attribute = {};
  attribute.shaderLocation = shader_location;
  attribute.offset = offset;
  attribute.format = format;
  return attribute;
}