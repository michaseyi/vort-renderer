#pragma once

#include <fastgltf/core.hpp>
#include <filesystem>

fastgltf::Asset load_gltf(std::filesystem::path path);
