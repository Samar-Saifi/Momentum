//
// Created by mdsamar on 10/10/26.
//

#ifndef MOMENTUM_MATERIAL_H
#define MOMENTUM_MATERIAL_H
#include <memory>

#include "Texture.h"
#include "Shader.h"

class Material {
public:
    Material(std::shared_ptr<Shader> shader) : m_Shader(shader) {};

    void SetColor(glm::vec4 color) { m_Color = color; }

    void Bind() {
        if (m_Shader) {
            m_Shader->Bind();
            m_Shader->SetVec4("u_Color", m_Color);
            if (m_Texture) {
                m_Texture->Bind(0);
            } else {
                GetDefaultTexture()->Bind(0);
            }
        }
    }

    void SetTexture(std::shared_ptr<Texture> texture = nullptr) { m_Texture = texture; }
    std::shared_ptr<Shader> GetShader() const { return m_Shader; }

    static std::shared_ptr<Texture> GetDefaultTexture() {
        static std::shared_ptr<Texture> s_DefaultTexture = std::make_shared<Texture>(1, 1, 0xFFFFFFFF);
        return s_DefaultTexture;
    }

private:
    std::shared_ptr<Shader> m_Shader;
    glm::vec4 m_Color = glm::vec4(1.0f);
    std::shared_ptr<Texture> m_Texture;
};

#endif //MOMENTUM_MATERIAL_H
