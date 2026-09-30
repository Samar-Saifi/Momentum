//
// Created by mdsamar on 17/09/26.
//

#include "Application.h"

#include <cassert>
#include <iostream>
#include "Window.h"
#include <glad/glad.h>

#include "DeltaTime.h"
#include "ImGuiManager.h"
#include "GLFW/glfw3.h"

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name) {
    assert(!s_Instance && "Application instance already exists!");
    s_Instance = this;

    WindowProperties properties;
    properties.title = name;

    m_Window = std::make_unique<Window>(properties);
    m_Window->SetEventCallback([this](EventBase& e) {
        OnEvent(e);
    });

    m_ImGuiManager = std::make_unique<ImGuiManager>();
    m_ImGuiManager->Init(m_Window->GetWindow());

    std::cout << "MOMEMTUM INITIALIZED SUCCESSFULLY" << std::endl;
}

void Application::Quit() {
    m_Running = false;
}

void Application::OnEvent(EventBase &event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {return OnWindowClose(e);});
    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e);});

    if (!event.handled && m_ImGuiManager) {
        m_ImGuiManager->OnEvent(event);
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }
    m_Minimized = false;
    glViewport(0, 0, e.GetWidth(), e.GetHeight());
    return false;
}

void Application::Run() {
    OnStart();

    m_LastFrameTime = static_cast<float>(glfwGetTime());

    while (m_Running) {
        float time = static_cast<float>(glfwGetTime());
        DeltaTime dt = time-m_LastFrameTime;
        m_LastFrameTime = time;
        OnProcessInput();
        if (!m_Minimized) {
            m_Timer += dt.GetSeconds();
            while (m_Timer > m_FixedDeltaTime) {
                OnFixedUpdate(m_FixedDeltaTime);
                m_Timer -= m_FixedDeltaTime;
            }

            OnUpdate(dt);

            glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            OnRender();

            if (m_ImGuiManager) {
                m_ImGuiManager->Begin();
                OnRenderImGui();
                m_ImGuiManager->End();
            }
        }
        m_Window->OnUpdate();
    }
    OnShutdown();
}

Application::~Application() {
    if (m_ImGuiManager) {
        m_ImGuiManager->Shutdown();
    }
    s_Instance = nullptr;
}
