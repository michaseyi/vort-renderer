#pragma once
#include <array>
#include "buffer_range.h"
#include "wgpu.h"

struct RenderableObjectData {
  std::array<WGPUBuffer, 5> vertex_buffers;
  std::array<WGPUVertexBufferLayout, 5> vertex_buffer_layouts;
  std::array<BufferRange, 5> vertex_buffer_ranges;
  u32 vertex_buffer_count;
  WGPUBuffer index_buffer;
  BufferRange index_buffer_range;
  WGPUIndexFormat index_format;
  WGPUShaderModule vertex_shader;
  const char* vertex_shader_entry_point;
  u32 vertex_shader_invoke_count;

  WGPUPrimitiveTopology primitive_topology;

  WGPUShaderModule fragment_shader;
  const char* fragment_shader_entry_point;
};
