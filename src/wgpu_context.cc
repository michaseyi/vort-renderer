#include "wgpu_context.h"
#include <cassert>
#include <iostream>

#include "wgpu_initializers.h"
#include "wgpu_utils.h"

WGPUContext WGPUContext::context_{};

bool WGPUContext::init() {
  WGPUInstanceDescriptor instance_descriptor = wgpuinit::instance_descriptor();
  context_.instance = wgpuutils::create_instance(&instance_descriptor);

  if (context_.instance == nullptr) {
    cleanup();
    return false;
  }

  WGPURequestAdapterOptions request_adapter_options =
      wgpuinit::request_adapter_options();

  context_.adapter =
      wgpuutils::request_adapter(context_.instance, &request_adapter_options);

  if (context_.adapter == nullptr) {
    cleanup();
    return false;
  }

  WGPUDeviceDescriptor device_descriptor = wgpuinit::device_descriptor();
  context_.device =
      wgpuutils::request_device(context_.adapter, &device_descriptor);

  if (context_.device == nullptr) {
    cleanup();
    return false;
  }

  context_.queue = wgpuDeviceGetQueue(context_.device);

  if (context_.queue == nullptr) {
    cleanup();
    return false;
  }

  wgpuDeviceSetUncapturedErrorCallback(
      context_.device,
      [](WGPUErrorType, const char* message, void*) {
        std::cerr << "Error: " << message << std::endl;
      },
      nullptr);

  return true;
}

const WGPUContext& WGPUContext::get() {
  return context_;
}

void WGPUContext::cleanup() {
  if (context_.queue) {
    wgpuQueueRelease(context_.queue);
  }
  if (context_.device) {
    wgpuDeviceRelease(context_.device);
  }
  if (context_.adapter) {
    wgpuAdapterRelease(context_.adapter);
  }
  if (context_.instance) {
    wgpuInstanceRelease(context_.instance);
  }
}
