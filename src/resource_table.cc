#include "resource_table.h"
#include <cassert>

void ResourceTable::set(std::string_view resource_name, Entry entry) {
  table_[resource_name] = entry;
}

ResourceTable::Entry& ResourceTable::get(std::string_view resource_name) {
  auto result = table_.find(resource_name);
  assert(result != table_.end());
  return result->second;
}

ResourceTable::Entry ResourceTable::Entry::from_texture(WGPUTexture texture) {
  Entry entry;
  entry.type = Type::Texture;
  entry.texture_data.texture = texture;
  entry.texture_data.format = wgpuTextureGetFormat(texture);
  entry.texture_data.size = {wgpuTextureGetWidth(texture),
                             wgpuTextureGetHeight(texture), 0};
  return entry;
}

ResourceTable::Entry ResourceTable::Entry::from_buffer(WGPUBuffer buffer,
                                                       u64 offset, u64 size) {
  Entry entry;
  entry.type = Type::Buffer;
  entry.buffer_data.buffer = buffer;
  entry.buffer_data.offset = offset;
  entry.buffer_data.size = size;
  return entry;
}

ResourceTable::TextureData& ResourceTable::Entry::as_texture() {
  assert(type == Type::Texture);
  return texture_data;
}

ResourceTable::BufferData& ResourceTable::Entry::as_buffer() {
  assert(type == Type::Buffer);
  return buffer_data;
}