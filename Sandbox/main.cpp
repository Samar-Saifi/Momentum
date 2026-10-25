#include <filesystem>
#include <imgui_impl_opengl3_loader.h>
#include <iostream>
#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "../Engine/Renderer/Material.h"
#include "../Engine/Renderer/Mesh.h"
#include "Window.h"
#include "../cmake-build-debug/_deps/imgui-src/imgui.h"

#include "../Engine/Renderer/Renderer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/PrimitiveShapes.h"

struct SceneObject {
    std::string name;
    std::shared_ptr<Mesh> mesh;
    glm::vec3 postion = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
};

class SandboxApp : public Application {
public:
    SandboxApp() : Application() {}

    void AddCube(const std::string& name, glm::vec3 pos, glm::vec3 scale = glm::vec3(1.0f), glm::vec4 color = glm::vec4(1.0f)) {
        SceneObject obj;
        obj.name = name; obj.postion = pos; obj.color = color; obj.scale = scale;
        obj.mesh = PrimitiveShapes::CreateCube(1.0, m_Material);
        m_SceneObjects.push_back(obj);
        m_SelectedObjectIndex = (int) m_SceneObjects.size() - 1;
    }

    void OnStart() override {
        std::cout << "Sandbox started.\n";
        Renderer::Init();

        Input::BindKey("CameraZoomIn", GLFW_KEY_Q, "Gameplay");
        Input::BindKey("CameraUp", GLFW_KEY_W, "Gameplay");
        Input::BindKey("CameraZoomOut", GLFW_KEY_E, "Gameplay");
        Input::BindKey("CameraDown", GLFW_KEY_S, "Gameplay");
        Input::BindKey("CameraLeft", GLFW_KEY_A, "Gameplay");
        Input::BindKey("CameraRight", GLFW_KEY_D, "Gameplay");
        Input::BindKey("Jump", GLFW_KEY_SPACE, "Gameplay");


        m_Shader = std::make_shared<Shader>("../../Engine/Shaders/vert.shader", "../../Engine/Shaders/frag.shader");
        m_Material = std::make_shared<Material>(m_Shader);

        AddCube("RedCube", glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.2f), glm::vec4(0.85f, 0.25f, 0.25f, 1.0f));

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

        float moveSpeed = 30 * dt.GetSeconds();
        if (Input::ActionPressed("CameraZoomIn")) {m_Camera->ZoomCamera(moveSpeed);}
        if (Input::ActionPressed("CameraZoomOut")) {m_Camera->ZoomCamera(-moveSpeed);}
        if (Input::ActionPressed("CameraLeft")) {m_Camera->RotateCamera(moveSpeed, 0.0f);}
        if (Input::ActionPressed("CameraRight")) {m_Camera->RotateCamera(-moveSpeed, 0.0f);}
        if (Input::ActionPressed("CameraDown")) {m_Camera->RotateCamera(0.0f, -moveSpeed);}
        if (Input::ActionPressed("CameraUp")) {m_Camera->RotateCamera(0.0f, moveSpeed);}
    }

    void OnRender() override {
        Renderer::Clear(m_ClearColor);

        float aspect = ((float)GetWindow().GetWidth()/(float)GetWindow().GetHeight());
        glm::mat4 vp = m_Camera->GetProjectionMatrix(aspect) * m_Camera->GetViewMatrix();

        m_Shader->Bind();
        m_Shader->SetMat4("u_VPMat", vp);
        m_Shader->SetVec3("u_LightDir", glm::vec3(0.2f,0.5f,1));
        m_Shader->SetVec4("u_Color", glm::vec4(1.0f));

        for (const auto& obj : m_SceneObjects) {
            if (!obj.mesh || !obj.mesh->vertexArray) continue;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.postion);
            model = glm::rotate(model, glm::radians(glm::radians(obj.rotation.x)), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(glm::radians(obj.rotation.y)), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(glm::radians(obj.rotation.z)), glm::vec3(0.0f, 0.0f, 1.0f));

            m_Material->SetColor(obj.color);
            m_Material->Bind();

            Renderer::Draw(obj.mesh->vertexArray, m_Shader, model);
        }
    }

    void OnRenderImGui() override {
        ImGui::Begin("Scene Manager & Shapes");
        float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        ImGui::ColorEdit4("Clear Color", color);
        ImGui::Separator();

        ImGui::Text("Add 3D Primitives:");
        if (ImGui::Button("+ Cube")) AddCube("Cube_" + std::to_string(m_SceneObjects.size()), glm::vec3(0.0f, 0.5f, 0.0f));
        ImGui::Separator();

        ImGui::Text("Objects in Scene (%zu):", m_SceneObjects.size());
        if (m_SceneObjects.empty()) {
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "Scene is currently empty. Click a button above to add shapes!");
        } else {
            std::vector<const char*> itemNames;
            for (const auto& obj : m_SceneObjects) {
                itemNames.push_back(obj.name.c_str());
            }

            if (m_SelectedObjectIndex >= static_cast<int>(m_SceneObjects.size())) {
                m_SelectedObjectIndex = static_cast<int>(m_SceneObjects.size() - 1);
            }

            ImGui::Combo("Select Object", &m_SelectedObjectIndex, itemNames.data(), static_cast<int>(itemNames.size()));

            if (ImGui::Button("Delete Selected Object")) {
                m_SceneObjects.erase(m_SceneObjects.begin() + m_SelectedObjectIndex);
                if (m_SelectedObjectIndex > 0) m_SelectedObjectIndex--;
            }

            if (!m_SceneObjects.empty() && m_SelectedObjectIndex >= 0 && m_SelectedObjectIndex < static_cast<int>(m_SceneObjects.size())) {
                SceneObject& sel = m_SceneObjects[m_SelectedObjectIndex];
                ImGui::SeparatorText("Selected Object Properties");

                char nameBuf[64];
                strncpy(nameBuf, sel.name.c_str(), sizeof(nameBuf));
                nameBuf[sizeof(nameBuf) - 1] = '\0';
                if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
                    sel.name = nameBuf;
                }

                ImGui::DragFloat3("Position", glm::value_ptr(sel.postion), 0.1f);
                ImGui::DragFloat3("Rotation (Deg)", glm::value_ptr(sel.rotation), 1.0f);
                ImGui::DragFloat3("Scale", glm::value_ptr(sel.scale), 0.05f, 0.01f, 50.0f);
                ImGui::ColorEdit4("Color", glm::value_ptr(sel.color));
            }
        }
        ImGui::End();
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

    std::shared_ptr<VertexArray> m_VA;

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Material> m_Material;
    std::unique_ptr<Camera> m_Camera;

    std::vector<SceneObject> m_SceneObjects;
    int m_SelectedObjectIndex = -1;
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}