#include <ecs/ecs.hpp>
#include <iostream>
#include "render_system.h"
#include "utils.h"
#include "vertex.h"
#include "wgpu_context.h"
#include "window.h"

struct App {
  static void window_update(ecs::Global<Window, ecs::AppState>& global) {
    auto [window, app_state] = global;

    window.poll();

    if (!window.is_open()) {
      app_state.running = false;
    }
  }

  static void window_cleanup() { glfwTerminate(); }

  static void window_setup(ecs::Command command) {
    auto init_status = glfwInit();
    assert(init_status);
    command.set_global(Window("Pose Interpolator"));
  }

  static void scene_setup(ecs::Command command) {
    fastgltf::Asset asset = load_gltf(
        "/home/michaseyi/Projects/webgpu/assets/human_rigged_mesh/"
        "scene.gltf");

    fill_entities_with_asset(command.get_world(), asset);
  }

  void operator()(ecs::World& world) {
    world.add_systems(ecs::SystemSchedule::Startup, window_setup,
                      render_system_setup, scene_setup);
    world.add_systems(ecs::SystemSchedule::Update, render_system_update,
                      window_update);
    world.add_systems(ecs::SystemSchedule::Shutdown, render_system_cleanup,
                      window_cleanup);
  }
};

i32 main() {
  ecs::World().add_plugins(App()).run();
  return 0;
}
