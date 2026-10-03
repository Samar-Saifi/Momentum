#include <filesystem>
#include <iostream>
#include "Application.h"
#include "../cmake-build-debug/_deps/imgui-src/imgui.h"

#include "Renderer.h"
#include "glm/ext/matrix_transform.hpp"

class SandboxApp : public Application {
public:
    SandboxApp() : Application() {}

    void OnStart() override {
        std::cout << "Sandbox started.\n";
        Renderer::Init();

        m_Shader = std::make_shared<Shader>("../../Engine/Shaders/vert.shader", "../../Engine/Shaders/frag.shader");

        float vertices[] = {
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.2f, 0.2f, 0.2f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,  2, 3, 0,
            1, 5, 6,  6, 2, 1,
            5, 4, 7,  7, 6, 5,
            4, 0, 3,  3, 7, 4,
            3, 2, 6,  6, 7, 3,
            4, 5, 1,  1, 0, 4
        };

        m_VA = std::make_shared<VertexArray>();

        std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
        std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
        m_VA->SetBuffer(vb, ib, 7 * sizeof(float));
    }

    void OnProcessInput() override {m_ProcessedFrames++;}
    void OnFixedUpdate(float fixedDeltaTime) override {m_FixedUpdateCount ++; m_FixedTotalTime += fixedDeltaTime; }
    void OnUpdate(DeltaTime dt) override {
        m_FrameTimer += dt.GetSeconds();
        m_FrameCount ++; m_LastDeltaTime = dt.GetMilliseconds();
        if (m_FrameTimer >= 1) {
            m_FPS = static_cast<int>(m_FrameCount / m_FrameTimer);
            m_FrameTimer = 0;
            m_FrameCount = 0;
        }

        m_RotationAngle += dt.GetSeconds() * 50.0f;
    }

    void OnRender() override {
        Renderer::Clear(m_ClearColor);
        m_Shader->Bind();
        glm::mat4 rotationTransform = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle), glm::vec3(0.0f, 1.0f, 1.0f));
        Renderer::Draw(m_VA, m_Shader, rotationTransform);
    }

    void OnRenderImGui() override {
        ImGui::Begin("Momentum Sandbox");

        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Game Loop Execution Pipeline:");
        ImGui::BulletText("1. ProcessInput  (Ticks: %llu)", m_ProcessedFrames);
        ImGui::BulletText("2. FixedUpdate   (Ticks: %llu, Accum Time: %.2f s)", m_FixedUpdateCount, m_FixedTotalTime);
        ImGui::BulletText("3. Update        (FPS: %d, Delta: %.2f ms)", m_FPS, m_LastDeltaTime);
        ImGui::BulletText("4. Render        (Custom Viewport Clear Active)");
        ImGui::BulletText("5. ImGuiRender   (Active Viewport UI)");

        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "Fixed Timestep Settings:");
        int targetHz = static_cast<int>(1.0f / GetFixedDeltaTime());
        if (ImGui::SliderInt("Fixed Physics Hz", &targetHz, 10, 240)) {
            SetFixedDeltaTime(1.0f / static_cast<float>(targetHz));
        }

        ImGui::End();
    }

    void OnEvent(EventBase &e) override {
        if (e.GetGroup() == EventGroup::Keyboard) {
            std::cout << "[APP TRACE] Keyboard Event: " << e.GetName() << "\n";
        } else if (e.GetGroup() == EventGroup::Mouse) {
            std::cout << "[APP INFO] Mouse Event: " << e.GetName() << "\n";
        }
        Application::OnEvent(e);
    }

    void OnShutdown() override {
        std::cout << "SHUTTING DOWN... " << "\n";
    }

private:
    glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
    unsigned int m_FixedUpdateCount = 0;
    unsigned int m_ProcessedFrames = 0;
    float m_FixedTotalTime = 0;
    float m_FrameTimer = 0;
    int m_FPS = 0;
    unsigned int m_FrameCount = 0;
    float m_LastDeltaTime = 0;
    float m_RotationAngle = 0.0f;

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VA;
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}