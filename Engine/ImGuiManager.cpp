//
// Created by mdsamar on 30/09/26.
//

#include "ImGuiManager.h"

#include "../cmake-build-debug/_deps/imgui-src/imgui.h"
#include "../cmake-build-debug/_deps/imgui-src/backends/imgui_impl_glfw.h"
#include "../cmake-build-debug/_deps/imgui-src/backends/imgui_impl_opengl3.h"

void ImGuiManager::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGuiManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::OnEvent(EventBase &event) {
    if (m_BlockEvent) {
        ImGuiIO& io = ImGui::GetIO();
        event.handled |= event.IsMuseEvent() && io.WantCaptureMouse;
        event.handled |= event.IsKeyboardEvent() && io.WantCaptureKeyboard;
    }
}

void ImGuiManager::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::End() {
    ImGuiIO& io = ImGui::GetIO();
    int w, h;
    GLFWwindow* window = glfwGetCurrentContext();
    if (window) {
        glfwGetFramebufferSize(window, &w, &h);
        io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
