//
// Created by mdsamar on 02/10/26.
//

#ifndef MOMENTUM_RENDERER_H
#define MOMENTUM_RENDERER_H
#include "Shader.h"
#include "VertexArray.h"
#include "glm/glm.hpp"
#include "glm/vec4.hpp"

class Renderer {
public:
    static void Init();
    static void Clear(glm::vec4 color = glm::vec4(0.0f));
    static void Draw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader, const glm::mat4 &transform);
};

#endif //MOMENTUM_RENDERER_H
