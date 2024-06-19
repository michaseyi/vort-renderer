#include "types.h"
#include "utils.h"

fastgltf::Asset load_gltf(std::filesystem::path path) {

  static constexpr auto supportedExtensions =
      fastgltf::Extensions::KHR_mesh_quantization |
      fastgltf::Extensions::KHR_texture_transform |
      fastgltf::Extensions::KHR_materials_variants;

  fastgltf::Parser parser(supportedExtensions);

  constexpr auto gltfOptions = fastgltf::Options::DontRequireValidAssetMember |
                               fastgltf::Options::AllowDouble |
                               fastgltf::Options::LoadExternalBuffers |
                               fastgltf::Options::LoadExternalImages |
                               fastgltf::Options::GenerateMeshIndices;

  auto gltfFile = fastgltf::MappedGltfFile::FromPath(path);

  assert(bool(gltfFile) && "Failed to open glTF file");

  auto asset = parser.loadGltf(gltfFile.get(), path.parent_path(), gltfOptions);

  assert(asset.error() == fastgltf::Error::None && "Failed to load glTF");

  return std::move(asset.get());
}
