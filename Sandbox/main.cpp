#include <filesystem>
#include <imgui_impl_opengl3_loader.h>
#include <iostream>
#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "Window.h"
#include "../cmake-build-debug/_deps/imgui-src/imgui.h"

#include "../Engine/Renderer/Renderer.h"
#include "glm/ext/matrix_transform.hpp"

class SandboxApp : public Application {
public:
    SandboxApp() : Application() {}

    void OnStart() override {
        std::cout << "Sandbox started.\n";
        Renderer::Init();

        // Input::BindKey("RotateLeft", GLFW_KEY_A);
        // Input::BindKey("RotateRight", GLFW_KEY_B);

        auto shader = std::make_shared<Shader>("../../Engine/Shaders/vert.shader", "../../Engine/Shaders/frag.shader");
        std::shared_ptr<Material> mat = std::make_shared<Material>(shader);

        m_Texture = std::make_unique<Texture>(64,64,0xFF00FFFF);

        float vertices[] = {
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
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
        m_VA->SetBuffer(vb, ib, 5 * sizeof(float));
        m_Mesh = std::make_unique<Mesh>(m_VA, mat);
        m_Camera = std::make_unique<Camera>();
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

        // m_RotationAngle += dt.GetSeconds() * 50.0f;

        auto window = GetWindow().GetWindow();
        float moveSpeed = 30 * dt.GetSeconds();
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            m_Camera->ZoomCamera(moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            m_Camera->ZoomCamera(-moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            m_Camera->RotateCamera(moveSpeed * 2, 0);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            m_Camera->RotateCamera(-moveSpeed * 2, 0);
        }
    }

    void OnRender() override {
        Renderer::Clear(m_ClearColor);
        if (m_Texture) m_Texture->Bind();
        if (!m_Mesh || !m_Mesh->material) return;
        m_Mesh->material->Bind();
        auto shader = m_Mesh->material->GetShader();
        if (!shader) return;
        float aspect = ((float)GetWindow().GetWidth()/(float)GetWindow().GetHeight());
        glm::mat4 vp = m_Camera->GetProjectionMatrix(aspect) * m_Camera->GetViewMatrix();
        shader->SetMat4("u_VPMat", vp);
        glm::vec3 lightDir = glm::vec3(0.2f,0.5f,1);
        shader->SetVec3("u_LightDir", lightDir);
        glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        shader->SetVec4("u_Color", whiteColor);
        glm::mat4 rotationTransform = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle), glm::vec3(0.0f, 1.0f, 1.0f));
        Renderer::Draw(m_VA, shader, rotationTransform);
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
        ImGui::SliderFloat("Rotation Angle", &m_RotationAngle, 0.0f, 360.0f);
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

    std::shared_ptr<VertexArray> m_VA;

    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<Mesh> m_Mesh;
    std::unique_ptr<Camera> m_Camera;
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}