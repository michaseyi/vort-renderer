#pragma once
#include <deque>
#include <memory>
#include <unordered_map>
#include <vector>
#include "render_passes/render_pass.h"
#include "renderable_target.h"
#include "types.h"

class RenderGraph {
 public:
  void execute(ecs::Command command, RenderableTarget& target);

  operator std::string() const;

  struct Node {
    std::unique_ptr<RenderPass> pass;
    std::string name;
  };

  void add_dependency(u32 dependent, u32 dependency);

  u32 add_node(std::string name, std::unique_ptr<RenderPass> pass);

  void topological_sort();

  void configure(RenderableTarget &target);

 private:
  std::vector<Node> nodes_;
  std::vector<u32> sorted_nodes_;
  std::unordered_map<u32, std::vector<u32>> dependencies_;
  ResourceTable resource_table_;
};