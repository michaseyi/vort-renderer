#pragma once
#include "wgpu.h"

struct ResourceBindData {
  WGPUBindGroupEntry bind_group_entry;
  WGPUBindGroupLayoutEntry bind_group_layout_entry;
};
