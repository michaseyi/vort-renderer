#pragma once
#include <GLFW/glfw3.h>

#include <string_view>
#include "renderable_target.h"
#include "types.h"

class Window {
 public:
  Window() = default;

  Window(i32 width, i32 height, std::string_view title);
  Window(std::string_view title);

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window& operator=(Window&&);

  Window(Window&&);

  ~Window();
  bool is_open() const;
  void poll();

  OnScreenTarget create_render_target(WGPUInstance instance);

  i32 width() const;

  i32 height() const;

 private:
  void set_window_size();
  GLFWwindow* glfw_handle_;
  i32 width_;
  i32 height_;
};