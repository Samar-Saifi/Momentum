//
// Created by mdsamar on 03/10/26.
//

#ifndef MOMENTUM_BUFFFER_H
#define MOMENTUM_BUFFFER_H

class VertexBuffer {
public:
    VertexBuffer(float* vertices, unsigned int size);
    ~VertexBuffer();

    void Bind();
    void Unbind();

private:
    unsigned int m_ID;
};

class IndexBuffer {
public:
    IndexBuffer(unsigned int* indices, unsigned int size);
    ~IndexBuffer();

    void Bind();
    void Unbind();

    unsigned int GetSize() const { return m_Size > 0 ? m_Size - 1 : 0; }

private:
    unsigned int m_ID;
    unsigned int m_Size;
};

#endif //MOMENTUM_BUFFFER_H
