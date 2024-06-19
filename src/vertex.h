#pragma once
#include <array>
#include <ecs/entities.hpp>
#include <fastgltf/core.hpp>

#include <vector>

#include "constants.h"
#include "glm.h"
#include "renderable_object_data.h"
#include "wgpu_initializers.h"

template <typename... T>
struct match : T... {
  using T::operator()...;
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coord;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct SkinnedMeshVertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coord;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  std::array<f32, Constants::max_bones_influence> joint_weights;
  std::array<ecs::EntityId, Constants::max_bones_influence> joint_ids;
};

class SkinnedMesh {
  std::vector<SkinnedMeshVertex> vertices;
  std::vector<u32> indices;
  std::vector<ecs::EntityId> joints;
};

class Mesh {
  std::vector<u32> indices;
  std::vector<Vertex> vertices;
};

struct Transform {
  glm::vec3 translation;
  glm::vec3 scale;
  glm::quat rotation;

  static Transform fromTRS(fastgltf::TRS& trs) {
    Transform transform;
    std::memcpy(&transform.translation, &trs.translation, sizeof(glm::vec3));
    std::memcpy(&transform.scale, &trs.scale, sizeof(glm::vec3));
    std::memcpy(&transform.rotation, &trs.rotation, sizeof(glm::quat));

    return transform;
  }
};

void load_skin_mesh(ecs::Entities& entities, fastgltf::Asset& asset,
                    ecs::EntityId entity_id, u64 node_index) {

  auto& node = asset.nodes[node_index];
  auto& mesh = asset.meshes[node.meshIndex.value()];
  auto& skin = asset.skins[node.skinIndex.value()];

  SkinnedMesh skinned_mesh;

  for (auto& primitive : mesh.primitives) {
    if (primitive.type != fastgltf::PrimitiveType::Triangles) {
      continue;
    }

    auto attribute = primitive.findAttribute("POSITION");
  }

  entities.set_components(entity_id, std::move(skinned_mesh));
}
void load_normal_mesh(ecs::Entities& entities, fastgltf::Asset& asset,
                      ecs::EntityId entity_id, u64 node_index) {
  auto& node = asset.nodes[node_index];
  auto& mesh = asset.meshes[node.meshIndex.value()];

  for (auto& primitive : mesh.primitives) {
    if (primitive.type != fastgltf::PrimitiveType::Triangles) {
      continue;
    }

    for (auto& [_, accessor] : primitive.attributes) {}
  }
  entities.set_components(entity_id, Mesh{});
}

void load_mesh(ecs::Entities& entities, fastgltf::Asset& asset,
               ecs::EntityId entity_id, u64 node_index) {
  auto& node = asset.nodes[node_index];

  if (!node.meshIndex.has_value()) {
    return;
  }

  if (!node.skinIndex.has_value()) {
    load_normal_mesh(entities, asset, entity_id, node_index);
  } else {
    load_skin_mesh(entities, asset, entity_id, node_index);
  }
}

void load_node(ecs::Entities& entities, fastgltf::Asset& asset,
               ecs::EntityId parent_id, u64 node_index) {
  auto& node = asset.nodes.at(node_index);

  ecs::EntityInterface interface = ecs::EntityInterface::None;

  Transform transform;

  std::visit(match{[&transform](fastgltf::TRS& trs) {
                     transform = Transform::fromTRS(trs);
                   },
                   [&transform](fastgltf::math::fmat4x4& matrix) {
                     fastgltf::TRS trs;
                     fastgltf::math::decomposeTransformMatrix(
                         matrix, trs.scale, trs.rotation, trs.translation);
                     transform = Transform::fromTRS(trs);
                   }},

             node.transform);
  auto node_entity_id = entities.create_entity_with(node.name.data(), interface,
                                                    parent_id, transform);

  load_mesh(entities, asset, node_entity_id, node_index);
  for (auto& child : node.children) {
    load_node(entities, asset, node_entity_id, child);
  }
}

void fill_entities_with_asset(ecs::Entities& entities, fastgltf::Asset& asset) {
  assert(asset.defaultScene.has_value());

  auto default_scene = asset.scenes.at(asset.defaultScene.value());
  auto scene_entity_id = entities.create_entity(default_scene.name.data(),
                                                ecs::EntityInterface::Scene);

  for (auto& node : default_scene.nodeIndices) {
    load_node(entities, asset, scene_entity_id, node);
  }
}
