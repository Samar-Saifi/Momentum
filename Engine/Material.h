//
// Created by mdsamar on 10/10/26.
//

#ifndef MOMENTUM_MATERIAL_H
#define MOMENTUM_MATERIAL_H
#include <memory>
#include "Renderer/Shader.h"

class Material {
public:
    Material(std::shared_ptr<Shader> shader) : m_Shader(shader) {};

    void SetColor(glm::vec4& color) { m_Color = color; }

    void Bind() {
        if (m_Shader) {
            m_Shader->Bind();
            m_Shader->SetVec4("u_Color", m_Color);
        }
    }

    std::shared_ptr<Shader> GetShader() const { return m_Shader; }

private:
    std::shared_ptr<Shader> m_Shader;
    glm::vec4 m_Color = glm::vec4(1.0f);
};

#endif //MOMENTUM_MATERIAL_H
