#include "render_graph.h"
#include "constants.h"
#include "wgpu_context.h"

#include <algorithm>
#include <memory>
#include <sstream>

void RenderGraph::execute(ecs::Command command, RenderableTarget& target) {
  resource_table_.set(
      Constants::output_texture,
      ResourceTable::Entry::from_texture(target.get_current_texture()));

  auto& context = WGPUContext::get();

  WGPUCommandEncoderDescriptor command_encoder_desc = {};
  WGPUCommandEncoder command_encoder =
      wgpuDeviceCreateCommandEncoder(context.device, &command_encoder_desc);

  for (u32 node_index : sorted_nodes_) {
    nodes_[node_index].pass->execute(command_encoder, command, resource_table_);
  }

  WGPUCommandBufferDescriptor command_buffer_desc = {};
  WGPUCommandBuffer command_buffer =
      wgpuCommandEncoderFinish(command_encoder, &command_buffer_desc);
  wgpuQueueSubmit(context.queue, 1, &command_buffer);

  wgpuCommandBufferRelease(command_buffer);
  wgpuCommandEncoderRelease(command_encoder);

  target.present();
}

void RenderGraph::configure(RenderableTarget& target) {
  ResourceTable::Entry entry;
  entry.type = ResourceTable::Type::Texture;
  entry.texture_data.format = target.get_format();
  entry.texture_data.size = target.get_extent();
  entry.texture_data.texture = nullptr;  // the texture itself should not be needed during configuration

  resource_table_.set(Constants::output_texture, entry);

  for (u32 node_index : sorted_nodes_) {
    nodes_[node_index].pass->configure(resource_table_);
  }
}

RenderGraph::operator std::string() const {
  std::stringstream ss;

  for (u32 node_index : sorted_nodes_) {
    ss << nodes_[node_index].name << " -> ";
    for (u32 dependency : dependencies_.at(node_index)) {
      ss << nodes_[dependency].name << ", ";
    }
    ss << "\n";
  }
  return ss.str();
}

void RenderGraph::add_dependency(u32 dependent, u32 dependency) {
  dependencies_[dependent].push_back(dependency);
}

u32 RenderGraph::add_node(std::string name, std::unique_ptr<RenderPass> pass) {
  nodes_.emplace_back(std::move(pass), name);
  return nodes_.size() - 1;
}

void RenderGraph::topological_sort() {
  std::vector<u32> in_degree(nodes_.size(), 0);
  for (const auto& [_, dependencies] : dependencies_) {
    for (u32 dependency : dependencies) {
      in_degree[dependency]++;
    }
  }

  std::deque<u32> queue;

  for (u32 i = 0; i < nodes_.size(); i++) {
    if (in_degree[i] == 0) {
      queue.push_back(i);
    }
  }

  sorted_nodes_.clear();
  sorted_nodes_.reserve(nodes_.size());

  while (!queue.empty()) {
    u32 node = queue.back();
    queue.pop_back();
    sorted_nodes_.push_back(node);

    for (u32 dependency : dependencies_[node]) {
      in_degree[dependency]--;
      if (in_degree[dependency] == 0) {
        queue.push_back(dependency);
      }
    }
  }

  std::reverse(sorted_nodes_.begin(), sorted_nodes_.end());
}
