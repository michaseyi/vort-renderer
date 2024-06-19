#include "window.h"
#include <cassert>

Window::Window(i32 width, i32 height, std::string_view title) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  glfw_handle_ =
      glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  assert(glfw_handle_);
  set_window_size();
}

Window::Window(std::string_view title) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  assert(monitor != nullptr);
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfw_handle_ = glfwCreateWindow(mode->width, mode->height, title.data(),
                                  nullptr, nullptr);
  assert(glfw_handle_);
  set_window_size();
}

i32 Window::width() const {
  return width_;
}

i32 Window::height() const {
  return height_;
}

Window& Window::operator=(Window&& other) {
  glfw_handle_ = other.glfw_handle_;
  width_ = other.width_;
  height_ = other.height_;
  other.glfw_handle_ = nullptr;

  return *this;
}

void Window::set_window_size() {
  glfwGetWindowSize(glfw_handle_, &width_, &height_);
}

OnScreenTarget Window::create_render_target(WGPUInstance instance) {
  return OnScreenTarget::from_glfw_window(instance, glfw_handle_);
}

Window::Window(Window&& other) {
  glfw_handle_ = other.glfw_handle_;
  width_ = other.width_;
  height_ = other.height_;
  other.glfw_handle_ = nullptr;
}

Window::~Window() {
  if (glfw_handle_) {
    glfwDestroyWindow(glfw_handle_);
  }
}

bool Window::is_open() const {
  return !glfwWindowShouldClose(glfw_handle_);
}

void Window::poll() {
  glfwPollEvents();
}