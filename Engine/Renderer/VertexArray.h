//
// Created by mdsamar on 03/10/26.
//

#ifndef MOMENTUM_VERTEXARRAY_H
#define MOMENTUM_VERTEXARRAY_H
#include <memory>

#include "Buffer.h"


class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void Unbind();

    void SetBuffer(std::shared_ptr<VertexBuffer>& vb, std::shared_ptr<IndexBuffer>& ib, unsigned int stride);
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

private:
    unsigned int m_ID = 0;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
};


#endif //MOMENTUM_VERTEXARRAY_H
