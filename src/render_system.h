#pragma once
#include <ecs/ecs.hpp>
#include "render_graph.h"
#include "renderable_target.h"


struct RenderOperation {
  std::unique_ptr<RenderableTarget> target;
  RenderGraph render_graph;
};

void render_system_setup(ecs::Command command);

void render_system_update(ecs::Command command,
                          ecs::Global<std::vector<RenderOperation>>& global);

void render_system_cleanup();
