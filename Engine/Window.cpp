//
// Created by mdsamar on 17/09/26.
//

#include "Window.h"

#include <iostream>

#include "glad/glad.h"
#include  <GLFW/glfw3.h>

#include "Events/ApplicationEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"


static bool s_GLFWWindowCreated = false;

static void GLFWErrorCallback(int error, const char* description) {
    std::cerr << "[MOMENTUM ERROR] GLFW Error (" << error << "): " << description << "\n";
}

Window::Window(const WindowProperties& props) {
    Init(props);
}

Window::~Window() {
    Shutdown();
}

void Window::Init(const WindowProperties& props) {
    m_Data.width = props.width; m_Data.height = props.height; m_Data.title = props.title;
    std::cout << "Creating Window '" << props.title << "' (" << props.width << "x" << props.height << ")\n";
    if (!s_GLFWWindowCreated) {
        int success = glfwInit();
        if (!success) {
            std::cerr << "[MOMENTUM CRITICAL] Failed to initialize GLFW!\n";
            return;
        }
        glfwSetErrorCallback(GLFWErrorCallback);
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

    glfwSetWindowUserPointer(m_Window, &m_Data);

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        if (data.eventCallback) data.eventCallback(event);
    });

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        if (data.eventCallback) data.eventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                if (data.eventCallback) data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                if (data.eventCallback) data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                if (data.eventCallback) data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int code) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        KeyTypedEvent event((int)code);
        if (data.eventCallback) data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                if (data.eventCallback) data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                if (data.eventCallback) data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent event(xpos, ypos);
        if (data.eventCallback) data.eventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent event(xpos, ypos);
        if (data.eventCallback) data.eventCallback(event);
    });
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
