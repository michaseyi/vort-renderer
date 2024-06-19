#pragma once
#include <string>
#include "types.h"

namespace Constants {
constexpr u32 max_bones_influence = 10;
constexpr i32 invalid_bone_id = -1;
constexpr char scene_color_texture[] = "scene_render_texture";
constexpr char scene_depth_stencil_texture[] = "scene_depth_stencil_texture";
constexpr char output_texture[] = "output_texture";
constexpr u8 multisampling_level = 4;
}  // namespace Constants