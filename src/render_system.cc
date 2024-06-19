#include "render_system.h"
#include "wgpu_context.h"
#include "window.h"

void render_system_setup(ecs::Command command) {

  auto init_status = WGPUContext::init();
  assert(init_status);

  (void)init_status;

  auto& context = WGPUContext::get();

  WGPUAdapterProperties properties;
  wgpuAdapterGetProperties(context.adapter, &properties);
  std::cout << properties.name << std::endl;

  command.set_global(std::vector<RenderOperation>());

  if (command.has_global<Window>()) {
    auto [render_operations] =
        command.get_global<std::vector<RenderOperation>>();

    auto [window] = command.get_global<Window>();

    auto render_target = window.create_render_target(context.instance);

    RenderableTarget::Configuration configuration{};
    configuration.format = render_target.get_preferred_format();
    configuration.device = context.device;
    configuration.width = u32(window.width());
    configuration.height = u32(window.height());
    configuration.present_mode = WGPUPresentMode_Fifo;
    configuration.format_size = render_target.get_preferred_format_size();
    render_target.configure(&configuration);

    RenderGraph render_graph;
    render_graph.add_node("dummy", std::make_unique<DummyRenderPass>());

    render_graph.topological_sort();

    render_graph.configure(render_target);

    render_operations.push_back(
        {std::make_unique<OnScreenTarget>(std::move(render_target)),
         std::move(render_graph)});
  }
}

void render_system_update(ecs::Command command,
                          ecs::Global<std::vector<RenderOperation>>& global) {
  auto [render_operations] = global;

  for (auto& render_operation : render_operations) {
    render_operation.render_graph.execute(command, *render_operation.target);
  }
}

void render_system_cleanup() {
  WGPUContext::cleanup();
}
