//
// Created by mdsamar on 17/09/26.
//

#include "Application.h"
#include <iostream>
#include "Window.h"
#include <glad/glad.h>

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name) {
    if (s_Instance != nullptr) {
        std::cout << "Application already exists!" << std::endl;
        return;
    }

    WindowProperties properties;
    properties.title = name;

    m_Window = std::make_unique<Window>(properties);
}

void Application::Quit() {
    m_Running = false;
}

void Application::Run() {
    OnStart();

    while (m_Running) {
        OnUpdate();

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Window->OnUpdate();
    }

    OnShutdown();
}

Application::~Application() {
    s_Instance = nullptr;
}
