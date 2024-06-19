#pragma once
#include "deletion_queue.h"
#include "renderable_target.h"
#include "types.h"
#include "wgpu.h"

struct WGPUContext {
  WGPUInstance instance;
  WGPUAdapter adapter;
  WGPUDevice device;
  WGPUQueue queue;

  WGPUContext() = default;

  static const WGPUContext& get();

  static bool init();

  static void cleanup();

 private:
  static WGPUContext context_;
};
