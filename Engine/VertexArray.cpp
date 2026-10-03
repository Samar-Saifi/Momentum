//
// Created by mdsamar on 03/10/26.
//

#include <glad/glad.h>
#include "VertexArray.h"

#include <iostream>

VertexArray::VertexArray() {
    glCreateVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind() {
    glBindVertexArray(m_ID);
}

void VertexArray::Unbind() {
    glBindVertexArray(0);
}


void VertexArray::SetBuffer(std::shared_ptr<VertexBuffer> &vb, std::shared_ptr<IndexBuffer> &ib, unsigned int stride) {
    std::cout << "Buffer Set";
    Bind();
    vb->Bind();
    ib->Bind();
    m_IndexBuffer = ib;
    m_VertexBuffer = vb;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void*)(3 * sizeof(float)));
}
