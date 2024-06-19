#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include "types.h"
#include "wgpu.h"

class ResourceTable {
 public:
  enum class Type : u8 {
    Invalid = 0,
    Texture,
    Buffer,
  };

  struct TextureData {
    WGPUTexture texture;
    WGPUTextureFormat format;
    WGPUExtent3D size;
  };

  struct BufferData {
    WGPUBuffer buffer;
    u64 offset;
    u64 size;
  };

  struct Entry {
    Type type;
    union {
      TextureData texture_data;
      BufferData buffer_data;
    };
    TextureData& as_texture();
    BufferData& as_buffer();
    static Entry from_texture(WGPUTexture texture);

    static Entry from_buffer(WGPUBuffer buffer, u64 offset, u64 size);
  };

  ResourceTable() = default;

  void set(std::string_view resource_name, Entry entry);

  Entry& get(std::string_view resource_name);

 private:
  std::unordered_map<std::string_view, Entry> table_;
};
