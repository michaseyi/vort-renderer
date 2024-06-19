#pragma once

#include <array>
#include <ecs/ecs.hpp>
#include "../resource_table.h"
#include "../wgpu.h"
#include "../wgpu_initializers.h"

struct RenderPass {
  virtual void execute(WGPUCommandEncoder command_encoder, ecs::Command command,
                       ResourceTable& resource_table) const = 0;

  virtual void configure(ResourceTable& resource_table) = 0;

  virtual ~RenderPass() = default;
};

struct SceneRenderPass : public RenderPass {
  bool multisampled = true;
  bool hdr = true;
  WGPUTexture render_texture = nullptr;
  WGPUTexture depth_stencil_texture = nullptr;
  WGPUTexture multisample_texture = nullptr;
  WGPUExtent3D extent = {0, 0, 1};
  WGPUTextureFormat render_texture_format;
  WGPUTextureFormat depth_stencil_texture_format =
      WGPUTextureFormat_Depth24PlusStencil8;

  SceneRenderPass() = default;
  SceneRenderPass(const SceneRenderPass&) = delete;
  SceneRenderPass& operator=(const SceneRenderPass&) = delete;
  SceneRenderPass& operator=(SceneRenderPass&& other);

  SceneRenderPass(SceneRenderPass&& other);

  ~SceneRenderPass();

  void configure(ResourceTable& resource_table) override;

  void execute(WGPUCommandEncoder command_encoder, ecs::Command command,
               ResourceTable& resource_table) const override;

 private:
  void cleanup();

  void destroy_texture(WGPUTexture texture);
};

struct DummyRenderPass : public RenderPass {
  void configure(ResourceTable& resource_table) override;

  void execute(WGPUCommandEncoder command_encoder, ecs::Command command,
               ResourceTable& resource_table) const override;
};

struct PresentRenderPass : public RenderPass {
  void configure(ResourceTable& resource_table) override;

  void execute(WGPUCommandEncoder command_encoder, ecs::Command command,
               ResourceTable& resource_table) const override;
};