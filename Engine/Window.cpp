//
// Created by mdsamar on 17/09/26.
//

#include "Window.h"
#include "glad/glad.h"
#include  <GLFW/glfw3.h>


static bool s_GLFWWindowCreated = false;

Window::Window(const WindowProperties& props) {
    Init(props);
}

Window::~Window() {
    Shutdown();
}

void Window::Init(const WindowProperties& props) {
    m_WindowTitle = props.title;
    m_Width = props.width;
    m_Height = props.height;

    if (!s_GLFWWindowCreated) {
        int success = glfwInit();
        if (!success) {
            return;
        }

        s_GLFWWindowCreated = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
    if (!m_Window) { return; }
    glfwMakeContextCurrent(m_Window);

    int version = gladLoadGL();
    if (!version) { return; }
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}
