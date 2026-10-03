//
// Created by mdsamar on 02/10/26.
//

#include "Renderer.h"

#include "glad/glad.h"

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::Clear(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader, const glm::mat4 &transform) {
    shader->SetMat4("transform", transform);
    shader->Bind();
    va->Bind();

    unsigned count = va->GetIndexBuffer() ? va->GetIndexBuffer()->GetSize() : 0;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
