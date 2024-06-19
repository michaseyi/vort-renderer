add_executable(pose_interpolator)

target_compile_features(pose_interpolator PRIVATE cxx_std_20)

file(GLOB_RECURSE SOURCES "*.cc" "*.c")

target_compile_options(pose_interpolator PRIVATE -Wall -Wextra -Wpedantic)


target_sources(pose_interpolator PUBLIC ${SOURCES})
target_link_directories(pose_interpolator PRIVATE ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(pose_interpolator PRIVATE fmt::fmt glfw wgpu_native.a fastgltf vort-ecs)

target_include_directories(pose_interpolator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

